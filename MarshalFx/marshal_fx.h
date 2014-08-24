#include "marshal_fx_details.h"

namespace marshal_fx {

	// Marshals the specified object to a desired new type. The to and from types
	// can be CLR or C++ types. To customize marshaling behavior for your own type,
	// specialize marshal_as from the msclr::interop namespace. DO NOT specialize
	// this method -- if you do, marshaling collections that contain your custom type
	// will still not be supported.
	template <typename TTo, typename TFrom>
	TTo marshal_as(TFrom const& from)
	{
		return details::marshal_traits<TTo, TFrom>::marshal(from,
			details::type_tag_t<TFrom, TTo>(), // from tag
			details::type_tag_t<TTo, TFrom>()  // to tag
			);
	}

}