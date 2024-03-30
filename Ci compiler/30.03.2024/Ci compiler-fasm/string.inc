; By zeq52giw 25.03.2024
; ÷ей файл це зб≥рка функц≥й по обробц≥ тексту

macro _alloc _size{
  invoke LocalAlloc, LPTR, _size
}

macro _free _pMem{
  invoke LocalHandle, _pMem
  invoke LocalFree, eax
}

proc strlen uses esi, _str
  
  xor eax, eax
  mov esi, [_str]
  @@:
    cmp byte[esi+eax], 0
    je @f
    inc eax
    jmp @b
  @@:
  
  ; return eax - string length
ret
endp

proc wstrlen uses esi, _str
; зупин€ец€ т≥льки при 00 00 (2 байта нул≥в)

  xor eax, eax
  mov esi, [_str]
  @@:
    cmp word[esi + eax*2], 0
    je @f
    inc eax
    jmp @b
  @@:
  
  ; return eax - string length
ret
endp

proc strcopy uses ebx ecx edx esi edi, _str, _buf, _bufsize

  cmp [_buf], 0
  je .newBuf
  
  mov esi, [_str]
  mov edi, [_buf]
  
  mov edx, [_bufsize]
  dec edx
  
  jmp @f
  
  .newBuf:
    stdcall strlen, [_str]
    inc eax
    _alloc eax
    mov edi, eax
  @@:
  
  xor eax, eax
  xor ebx, ebx
  
  @@:
    mov al, byte[esi]
    test al, al
    jz @f
    
    cmp ebx, edx
    jae @f
    
    mov word[edi+ebx], ax
    inc ebx
    inc esi
    
    jmp @b
  @@:
  
  mov eax, edi
  
ret
endp

proc wstrcopy uses ebx ecx edx esi edi, _str, _buf, _bufsize
  
  cmp [_buf], 0
  je .newBuf
  
  mov edx, [_bufsize]
  mov edi, [_buf]
  
  jmp @f
  
  .newBuf:
    stdcall wstrlen, [_str]
    inc eax
    shl eax, 1
    _alloc eax
    xor edx, edx
    mov edi, eax
  @@:
  
  xor eax, eax
  xor ebx, ebx
  
  dec edx
  
  mov esi, [_str]
  
  @@:
    mov ax, word[esi]
    test ax, ax
    jz @f
    
    cmp ebx, edx
    jae @f
    
    mov dword[edi+ebx*2], eax
    inc ebx
    add esi, 2
    
    jmp @b
  @@:       

  mov eax, edi

ret
endp
                                    
proc strcmp uses ebx esi edi, _str1, _str2

  mov esi, [_str1]
  mov edi, [_str2]
  
  test esi, esi
  jz .noEqu
  test edi, edi
  jz .noEqu
  
  stdcall strlen, esi
  mov ebx, eax
  stdcall strlen, edi
  
  cmp ebx, eax ; strlen(_str1) != strlen(_str2)
  jne .noEqu
  
  xor ebx, ebx
  
  @@:
    mov al, byte[esi+ebx]
    mov ah, byte[edi+ebx]
    
    inc ebx
    
    test al, al
    jz .ok
    
    cmp ah, al
    jne .noEqu
    jmp @b
  .noEqu:
    xor eax, eax
    jmp .exit
  
  .ok:
    mov eax, 1

  .exit:

ret
endp

proc wstrcmp uses ebx esi edi, _str1, _str2

  mov esi, [_str1]
  mov edi, [_str2]
  
  test esi, esi
  jz .noEqu
  test edi, edi
  jz .noEqu
  
  stdcall wstrlen, esi
  mov ebx, eax
  stdcall wstrlen, edi
  
  cmp ebx, eax ; wstrlen(_str1) != wstrlen(_str2)
  jne .noEqu
  
  xor ebx, ebx
  
  @@:
    mov ax, word[esi+ebx]
    mov cx, word[edi+ebx]
    
    inc ebx
    
    test ax, ax
    jz .ok
    
    cmp cx, ax
    jne .noEqu
    jmp @b
  .noEqu:
    xor eax, eax
    jmp .exit
  
  .ok:
    mov eax, 1

  .exit:

ret
endp

proc stradd uses ebx ecx edx esi edi, _str1, _str2, _buf

  mov edi, [_buf]         ; if(_buf == 0) edi = _alloc( strlen(_str1) + strlen(_str2) + 1 )
  cmp edi, 0
  jne @f
    mov eax, [_str1]
    stdcall strlen, eax
    push eax
    mov ebx, eax
    mov eax, [_str2]
    stdcall strlen, eax
    push eax
    add eax, ebx
    inc eax
    
    _alloc eax
    mov edi, eax
  @@:

  xor ebx, ebx
  pop edx ; ecx = strlen(_str2)
  pop ecx ; edx = strlen(_str1)
  mov esi, [_str1]
  @@:
    mov al, byte[esi+ebx]
    mov byte[edi+ebx], al
    inc ebx
  loop @b
  
  mov ecx, edx
  mov edx, ebx
  xor ebx, ebx
  mov esi, [_str2]
  
  @@:
    mov al, byte[esi+ebx]
    mov byte[edi+edx], al
    inc ebx
    inc edx
  loop @b
  
  mov byte[edi+edx], 0
  
  mov eax, edi ; return [_buf]

ret
endp

proc wstradd uses ebx ecx edx esi edi, _str1, _str2, _buf

  mov edi, [_buf]         ; if(_buf == 0) edi = _alloc( strlen(_str1) + strlen(_str2) + 1 )
  cmp edi, 0
  jne @f
    mov eax, [_str1]
    stdcall wstrlen, eax
    push eax
    mov ebx, eax
    mov eax, [_str2]
    stdcall wstrlen, eax
    push eax
    add eax, ebx
    inc eax
    shl eax, 1
    
    _alloc eax
    mov edi, eax
  @@:

  xor ebx, ebx
  pop edx ; ecx = strlen(_str2)
  pop ecx ; edx = strlen(_str1)
  mov esi, [_str1]
  @@:
    mov ax, word[esi+ebx*2]
    mov word[edi+ebx*2], ax
    inc ebx
  loop @b
  
  mov ecx, edx
  mov edx, ebx
  xor ebx, ebx
  mov esi, [_str2]
  
  @@:
    mov ax, word[esi+ebx*2]
    mov word[edi+edx*2], ax
    inc ebx
    inc edx
  loop @b
  
  mov word[edi+edx*2], 0
  
  mov eax, edi ; return [_buf]

ret
endp