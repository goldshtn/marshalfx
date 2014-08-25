#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>
#include <type_traits>

namespace marshal_fx {

	namespace details {

		template <typename T>
		struct remove_hat
		{
			typedef T type;
		};
		template <typename T>
		struct remove_hat<T^>
		{
			typedef T type;
		};
		template <typename T> using remove_hat_t = typename remove_hat<T>::type;

		// Determines whether a type is an STL collection. We deem a type to be an STL
		// collection if it has a nested ::iterator type, and if it's not std::string
		// or std::wstring.
		template <typename T>
		struct is_stl_collection
		{
			struct yes { char buf[1]; };
			struct no { char buf[2]; };

			template <typename U> static no test(...);
			template <typename U> static yes test(typename U::iterator*);

			static const bool value =
				(sizeof(test<T>(0)) == sizeof(yes)) &&
				!std::is_same<std::string, std::remove_cv_t<T>>::value &&
				!std::is_same<std::wstring, std::remove_cv_t<T>>::value;
		};

		// Determines whether a type is a CLR collection. We deem a type to be a CLR
		// collection if it implements the IEnumerable interface, and if it's not the
		// System::String type.
		template <typename T>
		struct is_clr_collection :
			public std::integral_constant<
				bool,
				std::is_base_of<System::Collections::IEnumerable, remove_hat_t<T>>::value &&
				!std::is_same<System::String, remove_hat_t<T>>::value
			>
		{
		};

		// Determines whether a type is a CLR dictionary. We deem a type to be a CLR
		// dictionary if it implements the IDictionary interface.
		template <typename T>
		struct is_clr_dictionary :
			public std::integral_constant<
				bool, 
				std::is_base_of<System::Collections::IDictionary, remove_hat_t<T>>::value
			>
		{
		};

		struct tag_base {};
		struct stl_collection_tag : public tag_base {};
		struct clr_collection_tag : public tag_base {};
		struct clr_dictionary_tag : public clr_collection_tag {};
		struct same_tag : public tag_base {};
		struct fallback_tag : public tag_base {};

		// Obtains the type tag from a combination of from and to types. The type tag
		// is then use to disambiguate numerous overloads that perform the actual marshaling.
		template <typename TFrom, typename TTo = void>
		struct type_tag
		{
			using type = std::conditional_t<
				is_stl_collection<TFrom>::value,
				stl_collection_tag,
				std::conditional_t<
					is_clr_dictionary<TFrom>::value,
					clr_dictionary_tag,
					std::conditional_t<
						is_clr_collection<TFrom>::value,
						clr_collection_tag,
						std::conditional_t<
							std::is_same<std::remove_cv_t<TFrom>, std::remove_cv_t<TTo>>::value,
							same_tag,
							fallback_tag
						>
					>
				>
			>;
		};
		template <typename TFrom, typename TTo = void>
		using type_tag_t = typename type_tag<TFrom, TTo>::type;
		
		// Main class for marshaling operations. Contains multiple methods that marshal
		// a specific combination of categories of to and from types, using tags to determine
		// which method should be called.
		template <typename TTo, typename TFrom>
		struct marshal_traits
		{
			// No special marshaling behavior detected. Fall back to the VC-provided
			// marshaler (e.g. System::String -> std::string can be marshaled by it).
			static TTo marshal(TFrom const& from, fallback_tag, tag_base)
			{
				return msclr::interop::marshal_as<TTo>(from);
			}

			// Detects a marshaling situation where the to and from types are the same.
			// Simply pass through the value.
			static TTo marshal(TFrom const& from, same_tag, tag_base)
			{
				return from;
			}
			
			// Marshal an STL collection to something that is NOT a CLR dictionary.
			static TTo marshal(TFrom const& from, stl_collection_tag, tag_base)
			{
				auto result = gcnew remove_hat_t<TTo>();
				using elem_type_t = decltype(System::Linq::Enumerable::First(result));
				for (auto const& elem : from)
				{
					result->Add(marshal_as<elem_type_t>(elem));
				}
				return result;
			}

			// Marshal an STL collection to something that is a CLR dictionary.
			static TTo marshal(TFrom const& from, stl_collection_tag, clr_dictionary_tag)
			{
				auto result = gcnew remove_hat_t<TTo>();
				using KVP = decltype(System::Linq::Enumerable::First(result));
				System::Collections::Generic::ICollection<KVP>^ coll = result;
				for (auto const& elem : from)
				{
					coll->Add(marshal_as<KVP>(elem));
				}
				return result;
			}
			
			// Marshal a CLR collection to an STL collection that is assumed to have an
			// insert method.
			static TTo marshal(TFrom const& from, clr_collection_tag, tag_base)
			{
				TTo result;
				// Using the Microsoft-specific syntax here for enumerating an IEnumerable,
				// as opposed for the standard C++ "for (... : ...)".
				for each (auto element in from)
				{
					result.insert(result.end(), marshal_as<typename TTo::value_type>(element));
				}
				return result;
			}
		};

		// Specialization for marshaling a std::pair to a System::Collections::Generic::KeyValuePair.
		template <typename TToKey, typename TToValue, typename TFromFirst, typename TFromSecond>
		struct marshal_traits<
			System::Collections::Generic::KeyValuePair<TToKey, TToValue>,
			std::pair<TFromFirst, TFromSecond>
		>
		{
			using KVP = System::Collections::Generic::KeyValuePair<TToKey, TToValue>;

			static KVP marshal(std::pair<TFromFirst, TFromSecond> const& from, fallback_tag, tag_base)
			{
				return KVP(marshal_as<TToKey>(from.first), marshal_as<TToValue>(from.second));
			}
		};

		// Specialization for marshaling a System::Collections::Generic::KeyValuePair to a std::pair.
		template <typename TToFirst, typename TToSecond, typename TFromKey, typename TFromValue>
		struct marshal_traits<
			std::pair<TToFirst, TToSecond>,
			System::Collections::Generic::KeyValuePair<TFromKey, TFromValue>
		>
		{
			using KVP = System::Collections::Generic::KeyValuePair<TFromKey, TFromValue>;

			static std::pair<TToFirst, TToSecond> marshal(KVP const& from, fallback_tag, tag_base)
			{
				auto& from_nonconst = const_cast<KVP&>(from);
				return std::pair<TToFirst, TToSecond>(
					marshal_as<std::remove_cv_t<TToFirst>>(from_nonconst.Key),
					marshal_as<std::remove_cv_t<TToSecond>>(from_nonconst.Value)
					);
			}
		};

	}

}