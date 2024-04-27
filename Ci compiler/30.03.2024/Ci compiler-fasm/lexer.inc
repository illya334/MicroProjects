; Цей файл перетворює код у токени

proc lexer

  .getWord:
  xor eax, eax
  mov ebx, [fileIn.pMem]
  mov ecx, [fileIn.index]
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
  
  jmp .getWordLoop
  
  .getWordLoopEnd:
  
  stdcall strlen, [tmpMem.pMem] ; if( strlen( tmpMem ) == 0 )
  test eax, eax
  jnz @f
  
  @@:
  
  mov [fileIn.index], ecx
  
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
    jnz @f
      stdcall strcopy, esi, 0, 0
      mov [nowcom.nameAdr], eax
      jmp .endDatatypeTest
    @@:
                             
    mov [nowcom.datatype], cl
    
  .endDatatypeTest:
  
  jmp .getWord
  
  .exit:
  
ret
endp