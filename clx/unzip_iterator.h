/* ------------------------------------------------------------------------- */
/*
 *  unzip_iterator.h
 *
 *  Copyright (c) 2004 - 2009, clown. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    - No names of its contributors may be used to endorse or promote
 *      products derived from this software without specific prior written
 *      permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  Last-modified: Sat 06 Jun 2009 16:45:00 JST
 */
/* ------------------------------------------------------------------------- */
#ifndef CLX_UNZIP_ITERATOR_H
#define CLX_UNZIP_ITERATOR_H

#include "config.h"
#include <iterator>
#include <string>
#include "zip/unzip.h"
#include "unzip_stream.h"
#include "shared_ptr.h"

namespace clx {
	/* --------------------------------------------------------------------- */
	//  basic_unzip_iterator
	/* --------------------------------------------------------------------- */
	template <
		class CharT,
		class Traits = std::char_traits<CharT>
	>
	class basic_unzip_iterator : public std::iterator<std::input_iterator_tag, basic_unzip_stream<CharT, Traits> > {
	public:
		typedef basic_unzip_stream<CharT, Traits> stream_type;
		typedef shared_ptr<stream_type> stream_ptr;
		typedef unzFile handler_type;
		typedef std::basic_string<CharT, Traits> string_type;
		
		basic_unzip_iterator() :
			cur_(), handler_(NULL), pass_() {}
		
		basic_unzip_iterator(const basic_unzip_iterator& cp) :
			cur_(cp.cur_), handler_(cp.handler_), pass_(cp.pass_) {}
		
		basic_unzip_iterator& operator=(const basic_unzip_iterator& cp) {
			handler_ = cp.handler_;
			cur_ = cp.cur_;
			pass_ = cp.pass_;
			return *this;
		}
		
		template <class Unzip>
		basic_unzip_iterator(const Unzip& cp) :
			cur_(), handler_(cp.handler()), pass_(cp.password()) {
			this->create();
		}
		
		stream_type& operator*() { return *cur_; }
		stream_ptr& operator->() { return cur_; }
		
		basic_unzip_iterator& operator++() {
			if (cur_ && !cur_->path().empty()) {
				if (unzLocateFile(handler_, cur_->path().c_str(), 0) != UNZ_OK) {
					cur_ = stream_ptr();
					return *this;
				}
			}
			
			if (unzGoToNextFile(handler_) == UNZ_OK) return this->create();
			else cur_ = stream_ptr();
			return *this;
		}
		
		basic_unzip_iterator operator++(int) {
			return ++(*this);
		}
		
		friend bool operator==(const basic_unzip_iterator& lhs, const basic_unzip_iterator& rhs) {
			return lhs.cur_ == rhs.cur_;
		}
		
		friend bool operator!=(const basic_unzip_iterator& lhs, const basic_unzip_iterator& rhs) {
			return !(lhs == rhs);
		}
		
	private:
		stream_ptr cur_;
		handler_type handler_;
		string_type pass_;
		
		basic_unzip_iterator& create() {
			if (handler_) {
				if (cur_) cur_ = stream_ptr();
				
				int status = UNZ_OK;
				if (!pass_.empty()) status = unzOpenCurrentFilePassword(handler_, pass_.c_str());
				else status = unzOpenCurrentFile(handler_);
				if (status == UNZ_OK) cur_ = stream_ptr(new stream_type(handler_));
			}
			return *this;
		}
	};
}

#endif // CLX_UNZIP_ITERATOR_H
