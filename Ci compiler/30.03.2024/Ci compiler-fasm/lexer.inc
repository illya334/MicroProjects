; Цей файл перетворює код у токени

proc lexer

  local .cmdEqu db 0

  .getWordLoop:
  mov eax, [fileIn.pMem]
  add eax, [fileIn.index]
  stdcall getWord, eax
  
  add [fileIn.index], ecx
  
  cmp ecx, 0
  jne .cmdSpecSimbol
    ; getSimbol
    mov ebx, [fileIn.index]
    mov esi, [fileIn.pMem]
    mov al, byte[esi+ebx]
    
    cmp al, ' '
    jne @f
      inc [fileIn.index]
    @@:
    
    cmp al, '='
    jne @f
      mov [.cmdEqu], true
      inc [fileIn.index]
    @@:
    
    jmp .getWordLoop
  .cmdSpecSimbol:
  
  ; tmpMem - now word
  
  ; перевірка на тип данних
  mov esi, [tmpMem.pMem]
  mov edi, datatype
  xor ecx, ecx
  @@:
    push ecx esi edi
    stdcall strcmp, esi, edi
    test eax, eax
    jnz @f
    stdcall strlen, edi
    test eax, eax ; 14 - none => strlen = 0
    je @f          
    pop edi esi ecx
    add edi, eax
    inc edi
    inc ecx
  jmp @b
  @@:
    pop edi esi ecx
    
    test eax, eax ; it is not data type
    jnz .itIsNotDataType
    
      stdcall strcopy, esi, 0, 0
      cmp [.cmdEqu], true
      jne @f
        mov [nowcom.valueAdr], eax
        jmp .endDatatypeTest
      @@:
      
      ; НЕ є ніодною командою, значить це інм'я
      mov [nowcom.nameAdr], eax
      jmp .endDatatypeTest
    .itIsNotDataType:
                             
    mov [nowcom.datatype], cl
    
  .endDatatypeTest:
  jmp .getWordLoop
  
  .exit:
  
ret
endp

proc getWord, _str
  xor eax, eax
  mov ebx, [_str]
  xor ecx, ecx
  mov edx, -1
  mov esi, simbols
  mov edi, [tmpMem.pMem]
  .getWordLoop:
    mov al, byte[ebx+ecx]
    .getValidSimbolLoop:
      inc edx
      cmp byte[esi+edx], 0
      jne @f
        xor edx, edx
        jmp .getValidSimbolLoopEnd
      @@:
    cmp al, byte[esi+edx]          
    jne .getValidSimbolLoop
  .getValidSimbolLoopEnd:
    
  test edx, edx
  jz .getWordLoopEnd
    
  mov word[edi], ax ; = AL | 00
  inc edi
  inc ecx
  mov edx, -1
  
  jmp .getWordLoop
  
  .getWordLoopEnd:
ret
endp