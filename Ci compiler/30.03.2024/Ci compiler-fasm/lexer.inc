; Цей файл перетворює код у токени

proc lexer uses ebx ecx edx esi edi

  xor eax, eax
  mov ebx, [fileIn.pMem]
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
        mov edx, -1
        jmp .getValidSimbolLoopEnd
      @@:
    cmp al, byte[esi+edx]          
    jne .getValidSimbolLoop
  .getValidSimbolLoopEnd:
    
  cmp edx, -1
  je .getWordLoopEnd
    
  mov word[edi], ax ; = AL | 00
  inc edi
  inc ecx
  
  jmp .getWordLoop
  
  .getWordLoopEnd:
  
  
    
ret
endp