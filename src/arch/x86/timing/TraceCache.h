/*
 *  Multi2Sim
 *  Copyright (C) 2012  Rafael Ubal (ubal@ece.neu.edu)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef X86_ARCH_TIMING_TRACE_CACHE_H
#define X86_ARCH_TIMING_TRACE_CACHE_H

#include <lib/cpp/IniFile.h>

#include "Thread.h"

namespace x86
{

// Trace cache class
class TraceCache
{
private:

	// Name of the trace cache
	std::string name;

	// structure of trace cache entry
	struct TraceCacheEntry
	{
		// LRU counter 
		int counter;

		// Address of the first instruction in the trace 
		unsigned int tag;

		// Number of micro- and macro-instructions in the cache line 
		int uop_count;
		int mop_count;

		//trace->branch_mask |= 1 << trace->branch_count;
		//trace->branch_flags |= taken << trace->branch_count;

		// Number of branches in the trace 
		int branch_count;

		// Bit mask of 'branch_count' bits, with all bits set to one. 
		int branch_mask;

		// Bit mask of 'branch_count' bits. A bit set to one represents a
		// taken branch. The MSB corresponds to the last branch in the trace.
		// The LSB corresponds to the first branch in the trace.
		int branch_flags;

		// Address of the instruction following the last instruction in the
		// trace.
		unsigned int fall_through;

		// Address of the target address of the last branch in the trace 
		unsigned int target;

		// This field has to be the last in the structure.
		// It is a list composed of 'x86_trace_cache_trace_size' elements.
		// Each element contains the address of the micro-instructions in the trace.
		// Only if each single micro-instructions comes from a different macro-
		// instruction can this array be full.
		unsigned int mop_array[0];
	};

	// Trace cache lines ('sets' * 'assoc' elements) 
	std::unique_ptr<TraceCacheEntry[]> entry;

	// Temporary trace, progressively filled up in the commit stage,
	// and dumped into the trace cache when full.
	std::unique_ptr<TraceCacheEntry> temp;

	// Statistics 
	long long accesses = 0;
	long long hits = 0;
	long long num_fetched_uinst = 0;
	long long num_dispatched_uinst = 0;
	long long num_issued_uinst = 0;
	long long num_committed_uinst = 0;
	long long num_squashed_uinst = 0;
	long long trace_length_acc = 0;
	long long trace_length_count = 0;

	// Trace cache parameter
	static int present;
	static int num_sets;
	static int assoc;
	static int trace_size;
	static int branch_max;
	static int queue_size;

public:

	/// Constructor
	TraceCache(const std::string &name = "");

	/// Read trace cache configuration from configuration file
	static void ParseConfiguration(const std::string &section,
				misc::IniFile &config);

	/// Dump configuration
	void DumpConfiguration(std::ostream &os = std::cout);

};

}

#endif // X86_ARCH_TIMING_TRACE_CACHE_H