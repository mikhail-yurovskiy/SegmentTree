#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <type_traits>

namespace SegmentTree
{
	namespace Details
	{
		template <typename T>
		T roundup2k( T value )
		{
			static_assert( std::is_integral<T>::value, "Integral type required." );
			using UT = std::remove_cv_t<std::make_unsigned_t<T>>;
			auto v = static_cast<UT>( value );
			--v;
			v |= v >> 1;
			v |= v >> 2;
			v |= v >> 4;
			if (sizeof( v ) >= 2)
				v |= v >> 8;
			if (sizeof( v ) >= 4)
				v |= v >> 16;
			if (sizeof( v ) >= 8)
				v |= v >> 32;
			return static_cast<T>( ++v );
		}


		template <typename It, typename T, typename BinaryOp>
		auto get_range( It from, It to, It range_begin, It range_end, It level, It node, T level_size, BinaryOp op )
		{
			// Check complete match of the interval to the current range.
			if (from == range_begin && to == range_end)
				return *node;

			// Move down to the child nodes.
			level_size <<= 1;
			auto const node_index = (node - level) << 1;
			level -= level_size;
			node   = level + node_index;

			// Calculate child range.
			auto const range_mid = range_begin + ((range_end - range_begin) >> 1);

			// Check if the interval is completely owned by one of the child nodes.
			if (from >= range_mid)
				return get_range( from, to, range_mid, range_end, level, ++node, level_size, op );
			if (to <= range_mid)
				return get_range( from, to, range_begin, range_mid, level, node, level_size, op );

			// General case: divide interval by the child nodes.
			auto const left  = get_range( from, range_mid, range_begin, range_mid, level, node, level_size, op );
			auto const right = get_range( range_mid, to, range_mid, range_end, level, ++node, level_size, op );
			return op( left, right );
		}
	}  // namespace Details


	template <typename Container, typename BinaryOp>
	void prepare( Container& segment_tree, BinaryOp op )
	{
		// Round up source array length to the nearest power of two and
		// fill it up to the rounded length with the default elements.
		auto tree_len = Details::roundup2k( segment_tree.size() );
		segment_tree.resize( tree_len );
		segment_tree.reserve( tree_len << 1 );

		// Fill tree levels from the leafs to the root.
		for (auto it = segment_tree.begin(); tree_len > 1; tree_len >>= 1)
		{
			auto range_end = it + tree_len;
			while (it != range_end)
			{
				auto& left  = *it++;
				auto& right = *it++;
				// Reserve at the function begin must guarantee that
				// the iterator 'it' will remain valid.
				segment_tree.emplace_back( op( left, right ) );
			}
		}
	}


	template <typename Container, typename T, typename BinaryOp>
	void modify( Container& segment_tree, typename Container::iterator pos, T const& value, BinaryOp op )
	{
		// Modify the leaf itself.
		*pos = value;

		// Go up to the root modifying affected nodes.
		auto level_begin = segment_tree.begin();
		for (auto level_size  = (segment_tree.size() + 1) >> 1; level_size > 1; level_size >>= 1)
		{
			// Calculate position in the current tree level and find the sibling with the common parent.
			auto const position = pos - level_begin;
			auto const sibling  = level_begin + (position ^ 1);

			// Level up, find the new position.
			level_begin += level_size;
			auto new_pos = level_begin + (position >> 1);

			// Assign the new value and continue.
			*new_pos = op( *pos, *sibling );
			pos = new_pos;
		}
	}


	template <typename Container, typename BinaryOp>
	auto get_range( Container const& segment_tree, typename Container::const_iterator from, typename Container::const_iterator to, BinaryOp op )
	{
		// Begin from the root (the last element by design). The root range is the whole array.
		auto const range_begin = segment_tree.begin();
		auto const range_end   = range_begin + ((segment_tree.size() + 1) >> 1);
		auto const root        = segment_tree.end() - 1;
		return Details::get_range( from, to, range_begin, range_end, root, root, segment_tree.end() - root, op );
	}
}  // namespace SegmentTree

#endif  // SEGMENT_TREE_H
