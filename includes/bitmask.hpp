#pragma once

class Bitmask
{
	public:
		template <typename M, typename F>
		static bool HasFlag(M mask, F flag) { return ((mask & flag) == flag); }
};
