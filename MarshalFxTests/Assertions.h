namespace MarshalFxTests {

	template <typename TIn, typename TOut>
	static void MarshalAndAssert(TIn const& in, TOut const& expectedOut)
	{
		Assert::IsTrue(expectedOut == marshal_fx::marshal_as<TOut>(in));
	}

	template <typename TIn, typename TOut>
	static void MarshalAndAssertAreEqual(TIn const& in, TOut const& expectedOut)
	{
		Assert::AreEqual(expectedOut, marshal_fx::marshal_as<TOut>(in));
	}

	template <typename TIn, typename TOut>
	static void MarshalAndCLRCollectionAssert(TIn const& in, TOut const& expectedOut)
	{
		static_assert(
			marshal_fx::details::is_clr_collection<TOut>::value,
			"the output must be a CLR collection"
			);

		auto result = marshal_fx::marshal_as<TOut>(in);
		auto comparer = gcnew KellermanSoftware::CompareNetObjects::CompareLogic();
		auto cmpResult = comparer->Compare(expectedOut, result);
		Assert::IsTrue(
			cmpResult->AreEqual,
			"Objects not equal, differences:\n" + cmpResult->DifferencesString
			);
	}

	template <typename TIn, typename TOut>
	static void MarshalAndSTLCollectionAssert(TIn const& in, TOut const& expectedOut)
	{
		static_assert(
			marshal_fx::details::is_stl_collection<TOut>::value,
			"the output must be an STL collection"
			);

		auto result = marshal_fx::marshal_as<TOut>(in);
		Assert::AreEqual(expectedOut.size(), result.size()); // STL collections have .size()
		Assert::IsTrue(std::equal(
			expectedOut.begin(), expectedOut.end(),
			result.begin())
			);
	}

}
