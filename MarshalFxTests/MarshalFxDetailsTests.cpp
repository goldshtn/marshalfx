#include "stdafx.h"

#include "..\MarshalFx\marshal_fx_details.h"
#include <map>
#include <unordered_map>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>

using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace MarshalFxTests
{
	[TestClass]
	public ref struct MarshalFxDetailsTests
	{
		[TestMethod]
		void TestIsSTLCollection_Vector()
		{
			Assert::AreEqual(true, marshal_fx::details::is_stl_collection<std::vector<int>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_stl_collection<std::vector<std::string>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_stl_collection<std::vector<std::string const>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_stl_collection<std::vector<std::string> const>::value);
		}

		[TestMethod]
		void TestIsSTLCollection_Map()
		{
			Assert::AreEqual(true, marshal_fx::details::is_stl_collection<std::map<int, int>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_stl_collection<std::map<std::string, std::vector<int>>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_stl_collection<std::unordered_map<int, int>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_stl_collection<std::unordered_map<std::string, std::vector<int>>>::value);
		}

		[TestMethod]
		void TestIsSTLCollection_List()
		{
			Assert::AreEqual(true, marshal_fx::details::is_stl_collection<std::list<int>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_stl_collection<std::list<std::string>>::value);
		}

		[TestMethod]
		void TestIsSTLCollection_Set()
		{
			Assert::AreEqual(true, marshal_fx::details::is_stl_collection<std::set<int>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_stl_collection<std::set<std::string>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_stl_collection<std::unordered_set<int>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_stl_collection<std::unordered_set<std::string>>::value);
		}

		[TestMethod]
		void TestIsSTLCollection_Primitives()
		{
			Assert::AreEqual(false, marshal_fx::details::is_stl_collection<int>::value);
			Assert::AreEqual(false, marshal_fx::details::is_stl_collection<double>::value);
			Assert::AreEqual(false, marshal_fx::details::is_stl_collection<std::string>::value);
			Assert::AreEqual(false, marshal_fx::details::is_stl_collection<System::Decimal>::value);
			Assert::AreEqual(false, marshal_fx::details::is_stl_collection<System::Object^>::value);
			Assert::AreEqual(false, marshal_fx::details::is_stl_collection<System::Object>::value);
		}

		[TestMethod]
		void TestIsSTLCollection_CLRCollections()
		{
			Assert::AreEqual(false, marshal_fx::details::is_stl_collection<System::Collections::Generic::List<int>>::value);
			Assert::AreEqual(false, marshal_fx::details::is_stl_collection<System::Collections::Generic::List<int>^>::value);
			Assert::AreEqual(false, marshal_fx::details::is_stl_collection<System::Collections::Generic::Dictionary<int, int>>::value);
			Assert::AreEqual(false, marshal_fx::details::is_stl_collection<System::Collections::Generic::HashSet<int>>::value);
			Assert::AreEqual(false, marshal_fx::details::is_stl_collection<System::Collections::Generic::IEnumerable<int>>::value);
			Assert::AreEqual(false, marshal_fx::details::is_stl_collection<System::Collections::Generic::IEnumerable<int>^>::value);
			Assert::AreEqual(false, marshal_fx::details::is_stl_collection<System::Collections::IEnumerable>::value);
		}

		[TestMethod]
		void TestIsCLRCollection_List()
		{
			Assert::AreEqual(true, marshal_fx::details::is_clr_collection<System::Collections::Generic::List<int>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_clr_collection<System::Collections::Generic::List<int>^>::value);
			Assert::AreEqual(true, marshal_fx::details::is_clr_collection<System::Collections::Generic::List<System::String^>>::value);
		}

		[TestMethod]
		void TestIsCLRCollection_Dictionary()
		{
			Assert::AreEqual(true, marshal_fx::details::is_clr_collection<System::Collections::Generic::Dictionary<int, int>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_clr_collection<System::Collections::Generic::Dictionary<int, System::String^>>::value);
		}

		[TestMethod]
		void TestIsCLRCollection_HashSet()
		{
			Assert::AreEqual(true, marshal_fx::details::is_clr_collection<System::Collections::Generic::HashSet<int>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_clr_collection<System::Collections::Generic::HashSet<System::String^>>::value);
		}

		[TestMethod]
		void TestIsCLRCollection_IEnumerable()
		{
			Assert::AreEqual(true, marshal_fx::details::is_clr_collection<System::Collections::Generic::IEnumerable<int>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_clr_collection<System::Collections::Generic::IEnumerable<System::String^>>::value);
		}

		[TestMethod]
		void TestIsCLRCollection_Primitives()
		{
			Assert::AreEqual(false, marshal_fx::details::is_clr_collection<System::String>::value);
			Assert::AreEqual(false, marshal_fx::details::is_clr_collection<System::String^>::value);
			Assert::AreEqual(false, marshal_fx::details::is_clr_collection<int>::value);
			Assert::AreEqual(false, marshal_fx::details::is_clr_collection<System::Decimal>::value);
			Assert::AreEqual(false, marshal_fx::details::is_clr_collection<System::Object>::value);
		}

		[TestMethod]
		void TestIsCLRCollection_STLCollections()
		{
			Assert::AreEqual(false, marshal_fx::details::is_clr_collection<std::vector<int>>::value);
			Assert::AreEqual(false, marshal_fx::details::is_clr_collection<std::set<double>>::value);
			Assert::AreEqual(false, marshal_fx::details::is_clr_collection<std::string>::value);
		}
		
		[TestMethod]
		void TestIsCLRDictionary_Dictionaries()
		{
			Assert::AreEqual(true, marshal_fx::details::is_clr_dictionary<System::Collections::Generic::Dictionary<int, int>>::value);
			Assert::AreEqual(true, marshal_fx::details::is_clr_dictionary<System::Collections::Generic::Dictionary<int, int>^>::value);
			Assert::AreEqual(true, marshal_fx::details::is_clr_dictionary<System::Collections::Generic::SortedDictionary<int, int>>::value);
		}

		[TestMethod]
		void TestIsCLRDictionary_NotDictionaries()
		{
			Assert::AreEqual(false, marshal_fx::details::is_clr_dictionary<System::Collections::Generic::List<int>>::value);
			Assert::AreEqual(false, marshal_fx::details::is_clr_dictionary<System::Collections::Generic::HashSet<int>>::value);
			Assert::AreEqual(false, marshal_fx::details::is_clr_dictionary<System::String>::value);
			Assert::AreEqual(false, marshal_fx::details::is_clr_dictionary<std::map<int, int>>::value);
		}

		[TestMethod]
		void TestRemoveHat()
		{
			Assert::IsTrue(std::is_same<
				System::String,
				marshal_fx::details::remove_hat_t<System::String>
			>::value);
			Assert::IsTrue(std::is_same<
				System::String,
				marshal_fx::details::remove_hat_t<System::String^>
			>::value);
		}
		
		[TestMethod]
		void TestTypeTag_STLCollection()
		{
			Assert::IsTrue(std::is_same<
				marshal_fx::details::stl_collection_tag,
				marshal_fx::details::type_tag_t<std::vector<int>>
			>::value);
			Assert::IsTrue(std::is_same<
				marshal_fx::details::stl_collection_tag,
				marshal_fx::details::type_tag_t<std::map<int, std::string>>
			>::value);
			Assert::IsFalse(std::is_same<
				marshal_fx::details::stl_collection_tag,
				marshal_fx::details::type_tag_t<std::string>
			>::value);
			Assert::IsFalse(std::is_same<
				marshal_fx::details::stl_collection_tag,
				marshal_fx::details::type_tag_t<System::Collections::Generic::List<int>>
			>::value);
			Assert::IsFalse(std::is_same<
				marshal_fx::details::stl_collection_tag,
				marshal_fx::details::type_tag_t<double>
			>::value);
		}

		[TestMethod]
		void TestTypeTag_CLRDictionary()
		{
			Assert::IsTrue(std::is_same<
				marshal_fx::details::clr_dictionary_tag,
				marshal_fx::details::type_tag_t<System::Collections::Generic::Dictionary<int, int>>
			>::value);
			Assert::IsTrue(std::is_same<
				marshal_fx::details::clr_dictionary_tag,
				marshal_fx::details::type_tag_t<System::Collections::Generic::Dictionary<int, int>^>
			>::value);
			Assert::IsFalse(std::is_same<
				marshal_fx::details::clr_dictionary_tag,
				marshal_fx::details::type_tag_t<System::Collections::Generic::List<int>>
			>::value);
			Assert::IsFalse(std::is_same<
				marshal_fx::details::clr_dictionary_tag,
				marshal_fx::details::type_tag_t<std::map<int, int>>
			>::value);
			Assert::IsFalse(std::is_same<
				marshal_fx::details::clr_dictionary_tag,
				marshal_fx::details::type_tag_t<double>
			>::value);
		}

		[TestMethod]
		void TestTypeTag_CLRCollection()
		{
			Assert::IsTrue(std::is_same<
				marshal_fx::details::clr_collection_tag,
				marshal_fx::details::type_tag_t<System::Collections::Generic::List<int>>
			>::value);
			Assert::IsTrue(std::is_same<
				marshal_fx::details::clr_collection_tag,
				marshal_fx::details::type_tag_t<System::Collections::Generic::HashSet<int>^>
			>::value);
			Assert::IsFalse(std::is_same<
				marshal_fx::details::clr_collection_tag,
				marshal_fx::details::type_tag_t<System::Collections::Generic::Dictionary<int, int>>
			>::value);
			Assert::IsFalse(std::is_same<
				marshal_fx::details::clr_collection_tag,
				marshal_fx::details::type_tag_t<std::vector<int>>
			>::value);
			Assert::IsFalse(std::is_same<
				marshal_fx::details::clr_collection_tag,
				marshal_fx::details::type_tag_t<double>
			>::value);
		}

		[TestMethod]
		void TestTypeTag_Same()
		{
			Assert::IsTrue(std::is_same<
				marshal_fx::details::same_tag,
				marshal_fx::details::type_tag_t<int, int>
			>::value);
			Assert::IsTrue(std::is_same<
				marshal_fx::details::same_tag,
				marshal_fx::details::type_tag_t<int const, int>
			>::value);
			Assert::IsTrue(std::is_same<
				marshal_fx::details::same_tag,
				marshal_fx::details::type_tag_t<std::string const, std::string>
			>::value);
			Assert::IsTrue(std::is_same<
				marshal_fx::details::same_tag,
				marshal_fx::details::type_tag_t<System::Decimal, System::Decimal>
			>::value);
			Assert::IsFalse(std::is_same<
				marshal_fx::details::same_tag,
				marshal_fx::details::type_tag_t<int*, int>
			>::value);
			Assert::IsFalse(std::is_same<
				marshal_fx::details::same_tag,
				marshal_fx::details::type_tag_t<System::Int64, System::Int32>
			>::value);
		}

		[TestMethod]
		void TestTypeTag_Fallback()
		{
			Assert::IsTrue(std::is_same<
				marshal_fx::details::fallback_tag,
				marshal_fx::details::type_tag_t<std::pair<int, int>>
			>::value);
			Assert::IsTrue(std::is_same<
				marshal_fx::details::fallback_tag,
				marshal_fx::details::type_tag_t<System::Collections::Generic::KeyValuePair<int, int>>
			>::value);
			Assert::IsTrue(std::is_same<
				marshal_fx::details::fallback_tag,
				marshal_fx::details::type_tag_t<std::string>
			>::value);
			Assert::IsTrue(std::is_same<
				marshal_fx::details::fallback_tag,
				marshal_fx::details::type_tag_t<System::String^>
			>::value);
		}
	};
}