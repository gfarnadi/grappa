////////////////////////////////////////////////////////////////////////
// This file is part of Grappa, a system for scaling irregular
// applications on commodity clusters. 

// Copyright (C) 2010-2014 University of Washington and Battelle
// Memorial Institute. University of Washington authorizes use of this
// Grappa software.

// Grappa is free software: you can redistribute it and/or modify it
// under the terms of the Affero General Public License as published
// by Affero, Inc., either version 1 of the License, or (at your
// option) any later version.

// Grappa is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// Affero General Public License for more details.

// You should have received a copy of the Affero General Public
// License along with this program. If not, you may obtain one from
// http://www.affero.org/oagpl.html.
////////////////////////////////////////////////////////////////////////

#pragma once

#include "MessageBase.hpp"
#include "MessageBaseImpl.hpp"
#include "Message.hpp"
#include <stack>
#include <glog/logging.h>


namespace Grappa {
/// @addtogroup Communication
/// @{

namespace SharedMessagePool {
  void init();
  void activate();
  
  size_t estimate_footprint();
  size_t adjust_footprint(size_t target);
  
  void* alloc(size_t sz);
  void free(MessageBase * m, size_t sz);
}

// Same as message, but allocated on heap
template< typename T >
inline Message<T> * heap_message(Core dest, T t) {
  auto *m = new (SharedMessagePool::alloc(sizeof(Message<T>))) Message<T>(dest, t);
  m->delete_after_send();
  return m;
}

/// Message with payload, allocated on heap
template< typename T >
inline PayloadMessage<T> * heap_message(Core dest, T t, void * payload, size_t payload_size) {
  auto *m = new (SharedMessagePool::alloc(sizeof(PayloadMessage<T>)))
    PayloadMessage<T>(dest, t, payload, payload_size);
  m->delete_after_send();
  return m;
}

/// Same as message, but allocated on heap and immediately enqueued to be sent.
template< typename T >
inline Message<T> * send_heap_message(Core dest, T t) {
  auto *m = new (SharedMessagePool::alloc(sizeof(Message<T>))) Message<T>(dest, t);
  m->delete_after_send();
  m->enqueue();
  return m;
}

/// Message with payload, allocated on heap and immediately enqueued to be sent.
template< typename T >
inline PayloadMessage<T> * send_heap_message(Core dest, T t, void * payload, size_t payload_size) {
  auto *m = new (SharedMessagePool::alloc(sizeof(PayloadMessage<T>)))
    PayloadMessage<T>(dest, t, payload, payload_size);
  m->delete_after_send();
  m->enqueue();
  return m;
}

} // namespace Grappa
