/*
 * Copyright (C) 2016 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "CodeBlock.h"
#include "CodeBlockSet.h"

namespace JSC {

inline void CodeBlockSet::mark(const LockHolder& locker, void* candidateCodeBlock)
{
    ASSERT(m_lock.isLocked());
    // We have to check for 0 and -1 because those are used by the HashMap as markers.
    uintptr_t value = reinterpret_cast<uintptr_t>(candidateCodeBlock);
    
    // This checks for both of those nasty cases in one go.
    // 0 + 1 = 1
    // -1 + 1 = 0
    if (value + 1 <= 1)
        return;

    CodeBlock* codeBlock = static_cast<CodeBlock*>(candidateCodeBlock); 
    if (!m_oldCodeBlocks.contains(codeBlock) && !m_newCodeBlocks.contains(codeBlock))
        return;

    mark(locker, codeBlock);
}

inline void CodeBlockSet::mark(const LockHolder&, CodeBlock* codeBlock)
{
    if (!codeBlock)
        return;

    m_currentlyExecuting.add(codeBlock);
}

} // namespace JSC

