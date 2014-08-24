#include "stdafx.h"

#include "..\MarshalFx\marshal_fx.h"
#include "Assertions.h"

#include <algorithm>
#include <vector>
#include <map>
#include <set>

using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

struct point
{
	int x, y;
	point(int xx, int yy) : x(xx), y(yy) {}
};
bool operator==(point const& a, point const& b)
{
	return a.x == b.x && a.y == b.y;
}

value struct Point
{
	int x, y;
	Point(int xx, int yy)
	{
		x = xx;
		y = yy;
	}
	bool Equals(System::Object^ obj) override
	{
		Point other = (Point)obj;
		return *this == other;
	}
	static bool operator==(Point a, Point b)
	{
		return a.x == b.x && a.y == b.y;
	}
};

ref struct PointClass
{
	int x, y;
	PointClass(int xx, int yy)
	{
		x = xx;
		y = yy;
	}
	bool Equals(System::Object^ obj) override
	{
		PointClass^ other = dynamic_cast<PointClass^>(obj);
		if ((System::Object^)other == nullptr)
		{
			return false;
		}
		return this == other;
	}
	static bool operator==(PointClass^ a, PointClass^ b)
	{
		if (Object::ReferenceEquals(a, nullptr))
		{
			return Object::ReferenceEquals(b, nullptr);
		}
		if (Object::ReferenceEquals(b, nullptr))
			return false;

		return a->x == b->x && a->y == b->y;
	}
};

namespace msclr {
	namespace interop {
		template <>
		point marshal_as<point, Point>(Point const& from)
		{
			return point(from.x, from.y);
		}
		template <>
		Point marshal_as<Point, point>(point const& from)
		{
			return Point(from.x, from.y);
		}
		template <>
		point marshal_as<point, PointClass^>(PointClass^ const& from)
		{
			return point(from->x, from->y);
		}
		template <>
		PointClass^ marshal_as<PointClass^, point>(point const& from)
		{
			return gcnew PointClass(from.x, from.y);
		}
	}
}

namespace MarshalFxTests
{
	[TestClass]
	public ref struct MarshalFxTests
	{
		[TestMethod]
		void TestPrimitives_Int()
		{
			MarshalAndAssert<int, int>(-5, -5);
		}

		[TestMethod]
		void TestPrimitives_Unsigned()
		{
			MarshalAndAssert<unsigned, unsigned>(5, 5);
		}

		[TestMethod]
		void TestPrimitives_Double()
		{
			MarshalAndAssert<double, double>(5.3, 5.3);
		}

		[TestMethod]
		void TestSimple_CLRStringToCPPString()
		{
			MarshalAndAssert(gcnew System::String("string"), std::string("string"));
		}

		[TestMethod]
		void TestSimple_CPPStringToCLRString()
		{
			MarshalAndAssert(std::string("string"), gcnew System::String("string"));
		}

		[TestMethod]
		void TestExtensibility_PointStructCLRToCPP()
		{
			MarshalAndAssert(Point(5, 3), point(5, 3));
		}

		[TestMethod]
		void TestExtensibility_PointStructCPPToCLR()
		{
			MarshalAndAssert(point(5, 3), Point(5, 3));
		}

		[TestMethod]
		void TestExtensibility_PointClassCLRToCPP()
		{
			MarshalAndAssert(gcnew PointClass(5, 3), point(5, 3));
		}

		[TestMethod]
		void TestExtensibility_PointClassCPPToCLR()
		{
			MarshalAndAssert(point(5, 3), gcnew PointClass(5, 3));
		}
		
		[TestMethod]
		void TestExtensibility_ListPointStructCLRToCPP()
		{
			auto points = gcnew System::Collections::Generic::List<Point>();
			points->Add(Point(1, 2));
			points->Add(Point(3, 4));
			MarshalAndSTLCollectionAssert(points, std::vector<point> {
				point{1, 2},
				point{3, 4}
			});
		}

		[TestMethod]
		void TestExtensibility_VectorPointStructCPPToCLR()
		{
			auto points = gcnew System::Collections::Generic::List<Point>();
			points->Add(Point(1, 2));
			points->Add(Point(3, 4));
			MarshalAndCLRCollectionAssert(std::vector<point> {
				point{ 1, 2 },
				point{ 3, 4 }
			}, points);
		}

		[TestMethod]
		void TestExtensibility_ListPointClassCLRToCPP()
		{
			auto points = gcnew System::Collections::Generic::List<PointClass^>();
			points->Add(gcnew PointClass(1, 2));
			points->Add(gcnew PointClass(3, 4));
			MarshalAndSTLCollectionAssert(points, std::vector<point> {
				point{ 1, 2 },
				point{ 3, 4 }
			});
		}

		[TestMethod]
		void TestExtensibility_VectorPointClassCPPToCLR()
		{
			auto points = gcnew System::Collections::Generic::List<PointClass^>();
			points->Add(gcnew PointClass(1, 2));
			points->Add(gcnew PointClass(3, 4));
			MarshalAndCLRCollectionAssert(std::vector<point> {
				point{ 1, 2 },
				point{ 3, 4 }
			}, points);
		}

		[TestMethod]
		void TestExtensibility_DictionaryPointStructCLRToCPP()
		{
			auto points = gcnew System::Collections::Generic::Dictionary<int, Point>();
			points->Add(1, Point(1, 1));
			points->Add(2, Point(2, 2));
			MarshalAndSTLCollectionAssert(points, std::map<int, point> {
					{ 1, point{ 1, 1 } },
					{ 2, point{ 2, 2 } }
			});
		}

		[TestMethod]
		void TestExtensibility_MapPointStructCPPToCLR()
		{
			auto points = gcnew System::Collections::Generic::Dictionary<int, Point>();
			points->Add(1, Point(1, 1));
			points->Add(2, Point(2, 2));
			MarshalAndCLRCollectionAssert(std::map<int, point> {
					{ 1, point{ 1, 1 } },
					{ 2, point{ 2, 2 } }
			}, points);
		}

		[TestMethod]
		void TestExtensibility_DictionaryPointClassCLRToCPP()
		{
			auto points = gcnew System::Collections::Generic::Dictionary<int, PointClass^>();
			points->Add(1, gcnew PointClass(1, 1));
			points->Add(2, gcnew PointClass(2, 2));
			MarshalAndSTLCollectionAssert(points, std::map<int, point> {
					{ 1, point{ 1, 1 } },
					{ 2, point{ 2, 2 } }
			});
		}

		[TestMethod]
		void TestExtensibility_MapPointClassCPPToCLR()
		{
			auto points = gcnew System::Collections::Generic::Dictionary<int, PointClass^>();
			points->Add(1, gcnew PointClass(1, 1));
			points->Add(2, gcnew PointClass(2, 2));
			MarshalAndCLRCollectionAssert(std::map<int, point> {
					{ 1, point{ 1, 1 } },
					{ 2, point{ 2, 2 } }
			}, points);
		}

		[TestMethod]
		void TestCPPPair()
		{
			MarshalAndAssertAreEqual(
				std::pair<int, std::string>(5, "five"),
				System::Collections::Generic::KeyValuePair<int, System::String^>(5, "five")
				);
		}

		[TestMethod]
		void TestCLRKeyValuePair()
		{
			MarshalAndAssert(
				System::Collections::Generic::KeyValuePair<int, System::String^>(5, "five"),
				std::pair<int, std::string>(5, "five")
				);
		}

		[TestMethod]
		void TestCPPVectorToCLRList_Int()
		{
			auto numbers = gcnew System::Collections::Generic::List<int>();
			numbers->Add(1);
			numbers->Add(2);
			numbers->Add(3);
			MarshalAndCLRCollectionAssert(std::vector<int> {1, 2, 3}, numbers);
		}

		[TestMethod]
		void TestCLRListToCPPVector_Int()
		{
			auto numbers = gcnew System::Collections::Generic::List<int>();
			numbers->Add(1);
			numbers->Add(2);
			numbers->Add(3);
			MarshalAndSTLCollectionAssert(numbers, std::vector<int> {1, 2, 3});
		}

		[TestMethod]
		void TestCPPVectorToCLRList_String()
		{
			auto strings = gcnew System::Collections::Generic::List<System::String^>();
			strings->Add("one");
			strings->Add("two");
			MarshalAndCLRCollectionAssert(std::vector<std::string> {"one", "two"}, strings);
		}

		[TestMethod]
		void TestCLRListToCPPVector_String()
		{
			auto strings = gcnew System::Collections::Generic::List<System::String^>();
			strings->Add("one");
			strings->Add("two");
			MarshalAndSTLCollectionAssert(strings, std::vector<std::string> {"one", "two"});
		}

		[TestMethod]
		void TestCPPVectorToCLRList_Pair()
		{
			namespace SCG = System::Collections::Generic;
			auto pairs = gcnew SCG::List<SCG::KeyValuePair<int, int>>();
			pairs->Add(SCG::KeyValuePair<int, int>(1, 1));
			pairs->Add(SCG::KeyValuePair<int, int>(2, 2));
			MarshalAndCLRCollectionAssert(std::vector<std::pair<int, int>> {
				{1, 1}, { 2, 2 }
			}, pairs);
		}

		[TestMethod]
		void TestCLRListToCPPVector_KVP()
		{
			namespace SCG = System::Collections::Generic;
			auto pairs = gcnew SCG::List<SCG::KeyValuePair<int, int>>();
			pairs->Add(SCG::KeyValuePair<int, int>(1, 1));
			pairs->Add(SCG::KeyValuePair<int, int>(2, 2));
			MarshalAndSTLCollectionAssert(pairs, std::vector<std::pair<int, int>> {
				{1, 1}, { 2, 2 }
			});
		}

		[TestMethod]
		void TestCPPVectorToCLRHashSet_String()
		{
			auto strings = gcnew System::Collections::Generic::HashSet<System::String^>();
			strings->Add("one");
			strings->Add("two");
			MarshalAndCLRCollectionAssert(std::vector<std::string> {"one", "two"}, strings);
		}

		[TestMethod]
		void TestCLRHashSetToCPPVector_String()
		{
			auto strings = gcnew System::Collections::Generic::HashSet<System::String^>();
			strings->Add("one");
			strings->Add("two");
			MarshalAndSTLCollectionAssert(strings, std::vector<std::string> {"one", "two"});
		}
		
		[TestMethod]
		void TestCPPSetToCLRHashSet_String()
		{
			auto strings = gcnew System::Collections::Generic::HashSet<System::String^>();
			strings->Add("one");
			strings->Add("two");
			MarshalAndCLRCollectionAssert(std::set<std::string> {"one", "two"}, strings);
		}

		[TestMethod]
		void TestCLRHashSetToCPPSet_String()
		{
			auto strings = gcnew System::Collections::Generic::HashSet<System::String^>();
			strings->Add("one");
			strings->Add("two");
			MarshalAndSTLCollectionAssert(strings, std::set<std::string> {"one", "two"});
		}

		[TestMethod]
		void TestCPPMapToCLRDictionary_StringString()
		{
			auto dict = gcnew System::Collections::Generic::Dictionary<System::String^, System::String^>();
			dict->Add("one", "first");
			dict->Add("two", "second");
			MarshalAndCLRCollectionAssert(std::map<std::string, std::string> {
				{ "one", "first" }, { "two", "second" }
			}, dict);
		}

		[TestMethod]
		void TestCPPMapToCLRSortedDictionary_StringString()
		{
			auto dict = gcnew System::Collections::Generic::SortedDictionary<System::String^, System::String^>();
			dict->Add("one", "first");
			dict->Add("two", "second");
			MarshalAndCLRCollectionAssert(std::map<std::string, std::string> {
					{ "one", "first" }, { "two", "second" }
			}, dict);
		}

		[TestMethod]
		void TestCLRDictionaryToCPPMap_StringString()
		{
			auto dict = gcnew System::Collections::Generic::Dictionary<System::String^, System::String^>();
			dict->Add("one", "first");
			dict->Add("two", "second");
			MarshalAndSTLCollectionAssert(dict, std::map<std::string, std::string> {
				{ "one", "first" }, { "two", "second" }
			});
		}

		[TestMethod]
		void TestCPPVectorOfVectorsToCLRListOfLists()
		{
			namespace SCG = System::Collections::Generic;
			auto listOfLists = gcnew SCG::List<SCG::List<int>^>();
			auto list1 = gcnew SCG::List<int>();
			list1->Add(1);
			list1->Add(2);
			auto list2 = gcnew SCG::List<int>();
			list2->Add(3);
			list2->Add(4);
			listOfLists->Add(list1);
			listOfLists->Add(list2);
			MarshalAndCLRCollectionAssert(std::vector<std::vector<int>> {
				{ 1, 2 }, { 3, 4 }
			}, listOfLists);
		}

		[TestMethod]
		void TestCLRListOfListsToCPPVectorOfVectors()
		{
			namespace SCG = System::Collections::Generic;
			auto listOfLists = gcnew SCG::List<SCG::List<int>^>();
			auto list1 = gcnew SCG::List<int>();
			list1->Add(1);
			list1->Add(2);
			auto list2 = gcnew SCG::List<int>();
			list2->Add(3);
			list2->Add(4);
			listOfLists->Add(list1);
			listOfLists->Add(list2);
			MarshalAndSTLCollectionAssert(listOfLists, std::vector<std::vector<int>> {
				{ 1, 2 }, { 3, 4 }
			});
		}

		template <typename T>
		static void foo(T const& t)
		{
			System::Console::WriteLine("{0} elements in dictionary", t->Count);
			for each (auto elem in t)
			{
				System::Console::WriteLine("\t{0} elements in inner list", elem.Value->Count);
				for each (auto innerElem in elem.Value)
				{
					System::Console::WriteLine("\t\telem = {0}", innerElem);
				}
			}
		}

		[TestMethod]
		void TestCPPMapOfStringAndVectorToCLRDictionaryOfStringAndList()
		{
			namespace SCG = System::Collections::Generic;
			auto dict = gcnew SCG::Dictionary<System::String^, SCG::List<int>^>();
			auto list1 = gcnew SCG::List<int>();
			list1->Add(1);
			list1->Add(2);
			auto list2 = gcnew SCG::List<int>();
			list2->Add(3);
			list2->Add(4);
			dict->Add("one", list1);
			dict->Add("two", list2);
			auto result = marshal_fx::marshal_as<decltype(dict)>(std::map<std::string, std::vector<int>> {
					{ "one", { 1, 2 } }, { "two", { 3, 4 } }
			});
			MarshalAndCLRCollectionAssert(std::map<std::string, std::vector<int>> {
				{ "one", { 1, 2 } }, { "two", { 3, 4 } }
			}, dict);
		}

		[TestMethod]
		void TestCLRDictionaryOfStringAndListToCPPMapOfStringAndVector()
		{
			namespace SCG = System::Collections::Generic;
			auto dict = gcnew SCG::Dictionary<System::String^, SCG::List<int>^>();
			auto list1 = gcnew SCG::List<int>();
			list1->Add(1);
			list1->Add(2);
			auto list2 = gcnew SCG::List<int>();
			list2->Add(3);
			list2->Add(4);
			dict->Add("one", list1);
			dict->Add("two", list2);
			MarshalAndSTLCollectionAssert(dict, std::map<std::string, std::vector<int>> {
				{ "one", { 1, 2 } }, { "two", { 3, 4 } }
			});
		}

		[TestMethod]
		void TestCLRIEnumerableToCPPVector()
		{
			namespace SCG = System::Collections::Generic;
			auto list = gcnew SCG::List<int>();
			list->Add(1);
			list->Add(2);
			auto enumerable = static_cast<SCG::IEnumerable<int>^>(list);
			MarshalAndSTLCollectionAssert(enumerable, std::vector<int> { 1, 2 });
		}

		[TestMethod]
		void TestCPPWideStringToCLRString()
		{
			MarshalAndAssert(std::wstring{ L"string" }, gcnew System::String("string"));
		}

		[TestMethod]
		void TestCLRStringToCPPWideString()
		{
			MarshalAndAssert(gcnew System::String("string"), std::wstring{ L"string" });
		}
	};
}
