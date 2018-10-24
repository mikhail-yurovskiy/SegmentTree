#include "SegmentTree.h"
#include <bits/stdc++.h>
using namespace std;


int main()
{
	{
		vector<int> segtree = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
		auto const original_size = segtree.size();
		SegmentTree::prepare( segtree, plus<>() );

		bool result = true;
		for (auto i = segtree.begin(); i < segtree.begin() + original_size; ++i) for (auto j = i + 1; j <= segtree.begin() + original_size; ++j)
		{
			auto const range = SegmentTree::get_range( segtree, i, j, plus<>() );
			int expected = 0;
			for (auto k = i; k < j; ++k)
				expected += *k;
			if (range != expected)
			{
				result = false;
				printf( "%ld..%ld = %d [%d] [FAILURE]\n", i - segtree.begin(), j - segtree.begin(), range, expected );
			}
		}
		if (result)
			printf( "SUCCESS\n" );
	}

	{
		vector<int> segtree = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
		auto const original_size = segtree.size();
		auto maxop = []( int a, int b ) { return max( a, b ); };
		SegmentTree::prepare( segtree, maxop );

		bool result = true;
		for (auto i = segtree.begin(); i < segtree.begin() + original_size; ++i) for (auto j = i + 1; j <= segtree.begin() + original_size; ++j)
		{
			auto const range = SegmentTree::get_range( segtree, i, j, maxop );
			int expected = 0;
			for (auto k = i; k < j; ++k)
				expected = max( expected, *k );
			if (range != expected)
			{
				result = false;
				printf( "%ld..%ld = %d [%d] [FAILURE]\n", i - segtree.begin(), j - segtree.begin(), range, expected );
			}
		}
		if (result)
			printf( "SUCCESS\n" );
	}

	{
		vector<int> segtree = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
		auto const original_size = segtree.size();
		auto maxop = []( int a, int b ) { return max( a, b ); };
		SegmentTree::prepare( segtree, maxop );
		SegmentTree::modify( segtree, segtree.begin() + 5, 1, maxop );

		bool result = true;
		for (auto i = segtree.begin(); i < segtree.begin() + original_size; ++i) for (auto j = i + 1; j <= segtree.begin() + original_size; ++j)
			{
				auto const range = SegmentTree::get_range( segtree, i, j, maxop );
				int expected = 0;
				for (auto k = i; k < j; ++k)
					expected = max( expected, *k );
				if (range != expected)
				{
					result = false;
					printf( "%ld..%ld = %d [%d] [FAILURE]\n", i - segtree.begin(), j - segtree.begin(), range, expected );
				}
			}
		if (result)
			printf( "SUCCESS\n" );
	}

	return 0;
}
