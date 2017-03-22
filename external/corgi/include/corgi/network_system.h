// Copyright 2015 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CORGI_NETWORK_SYSTEM_H_
#define CORGI_NETWORK_SYSTEM_H_

#include "system.h"
#include <memory>

namespace corgi {
template <typename T>
class NetworkSystem : public System<T> {
 public:
	NetworkSystem() {}

	virtual bool IsNetworkSystem() { return true; }

	virtual void SetRewindBufferSize(int snapshot_count) override {
		rewind_buffer_.resize(snapshot_count);
		rewind_buffer_size_ = snapshot_count;
		most_recent_buffer_index_ = 0;
		for (size_t i = 0; i < rewind_buffer_size_; i++) {
			rewind_buffer_[i].is_valid = false;
		}
	}

	virtual void TakeRewindSnapshot(WorldTime timestamp) override {
		most_recent_buffer_index_ = (most_recent_buffer_index_ + 1) % rewind_buffer_size_;
		RewindBufferEntry& snapshot = rewind_buffer_[most_recent_buffer_index_];
		snapshot.timestamp = timestamp;
		snapshot.component_data = component_data_;
		snapshot.component_index_lookup = component_index_lookup_;
		snapshot.is_valid = true;
	}

		// x % y is negative, if x is negative.  This makes sure it is
		// always positive, so -2 % 10 = 8 instead of -2.
	inline int SafeMod(int dividend, int divisor) {
		return (divisor + (dividend % divisor)) % divisor;
	}

	virtual bool RewindToTimestamp(WorldTime timestamp) override {
		int start = most_recent_buffer_index_;
		// Find the most recent snapshot that is before the requested timestamp.
		bool done = false;
		while (!done) {
			RewindBufferEntry& snapshot = rewind_buffer_[most_recent_buffer_index_];
			// if we ever hit an invalid snapshot then that means we don't have
			// enough data to perform the rewind operation.  Return true and abort.
			if (!snapshot.is_valid) return true;
			if (snapshot.timestamp <= timestamp) {
				done = true;
				break;
			} else {
				snapshot.is_valid = false;
				most_recent_buffer_index_ = SafeMod(most_recent_buffer_index_ - 1, rewind_buffer_size_);
			}
		}
		component_data_ = rewind_buffer_[most_recent_buffer_index_].component_data;
		component_index_lookup_ = rewind_buffer_[most_recent_buffer_index_].component_index_lookup;
		return false;
	}

 protected:

	struct RewindBufferEntry {
		RewindBufferEntry() : is_valid(false) {}

		bool is_valid;
		WorldTime timestamp;
		std::vector<ComponentData> component_data;
		std::unordered_map<EntityIdType, ComponentIndex> component_index_lookup;
	};

	std::vector<RewindBufferEntry> rewind_buffer_;
	size_t most_recent_buffer_index_;
	size_t rewind_buffer_size_;
};

}  // corgi

#endif  // CORGI_NETWORK_SYSTEM_H_
