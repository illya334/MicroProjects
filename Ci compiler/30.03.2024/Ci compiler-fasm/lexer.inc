; Цей файл перетворює код у токени

proc lexer
  ; Основна частина Lexer-a. Завдяння цієї подпрограми розбити текст на токени, переобразувати його (якщо потрібно).
  
  ; In:
  ; fileIn struct - структура входного файла на обробку.
  
  ; Out: 
  ; 
  
  ; TODO:
  ; способ завершити обробляти блок (;)
  ; функція створення токена ^^^
  ; обробка функцій

  local .cmdEqu db 0

  ; Init
  cmp [tmpMem.pMem], 0
  je @f
    _free [tmpMem.pMem]
  @@:
  _alloc 40
  mov [tmpMem.pMem], eax
  mov [tmpMem.len], 40

  mov word[nowcom], 0     ; type, datatype
  mov dword[nowcom+2], 0  ; name
  mov dword[nowcom+6], 0  ; value
  
  mov [nowLine], 0

  .getWordLoop:
  mov eax, [fileIn.pMem]
  add eax, [fileIn.index]
  stdcall getWord, eax
  test eax, eax
  jz .testGetWord
    cmp eax, -1 ; End File - Кінець файла, звільняє пам'ять та виходить з lexer
    jne @f
      _free [fileIn.pMem]
      mov eax, error.fileEnd
      jmp error.lexer
      jmp .exit
    @@:
    
    stdcall getTypeCom, dword[.cmdEqu]
    stdcall createTocken
    
    mov ecx, 1
    
  .testGetWord:
  
  add [fileIn.index], ecx
  
  stdcall testSpecSimbol
  test edx, edx
  jz @f
    mov [.cmdEqu], true
    jmp .getWordLoop
  @@:
  ; tmpMem - now word
  
  ; перевірка на тип данних
  stdcall testDatatype
    
    test eax, eax ; it is not data type
    jnz .itIsNotDataType
    
      ; копіює текст з tmpMem.pMem у створену пам'ять
      stdcall strcopy, esi, 0, 0 ; esi - text, name, value
      cmp byte[.cmdEqu], true
      jne @f
        mov [nowcom.value], eax
        mov byte[.cmdEqu], false
        jmp .endDatatypeTest
      @@:
      
      ; НЕ є ніодною командою, значить це ім'я
      mov [nowcom.name], eax
      jmp .endDatatypeTest
    .itIsNotDataType:
                             
    mov [nowcom.datatype], cl
    
  .endDatatypeTest:
  jmp .getWordLoop
  
  .exit:
  
ret 4
endp

proc getWord, _str
  ; Виділяє слово
  
  ; In:
  ; _str - входная строка
  
  ; Out:
  ; EAX - 0 - Ok; (-1) - End File
  ; EAX - ';', ','
  
  ; ECX - offset _str
  ; [tmpMem.pMem] - output string
  
  push [_str]
  
  ; обраховує кінцесу адресу, щоб не вийти за розміри пам'яті.
  mov eax, [tmpMem.len]
  add eax, [tmpMem.pMem]
  mov [tmpVar.a], eax
  
  mov ebx, [_str]
  
  mov al, byte[ebx]
  
  test al, al
  jnz @f
    mov eax, -1
    jmp .exit
  @@:
  
  cmp al, ';'
  jne @f
    mov eax, ';'
    jmp .exit
  @@:
  cmp al, ','
  jne @f
    mov eax, ','
    jmp .exit
  @@:
  
  cmp byte[ebx], 13
  je .nextLine
  cmp byte[ebx], 10
  je .nextLine
  
  jmp .nextLineEnd
  .nextLine:
    cmp word[ebx], 0D0Ah
    jne @f
      mov eax, [_str]
      add eax, 2
      mov [_str], eax
      inc [nowLine]
    @@:
    
    cmp byte[ebx], 13
    je @f
    cmp byte[ebx], 10
    je @f
    
    jmp .nextLineEnd
    @@:
      inc [_str]
      inc [nowLine]
    
  .nextLineEnd:
  
  cmp byte[ebx], ' '
  jne .endSpace
    
    mov ebx, [_str]
    
    .spaceLoop:
    cmp byte[ebx], 0
    jne @f
      mov eax, -1
      jmp .exit
    @@:
    
    inc ebx
    
    cmp byte[ebx], ' '
    je .spaceLoop
    
    mov [_str], ebx
    
  .endSpace:
  
  xor eax, eax
  mov ebx, [_str]
  xor ecx, ecx ; offset _str
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
  
  
  ; переповнення пам'яті, перевиділяє пам'ять - збільшує її розмір на 20 байт,
  ; перераховує кінцеву адресу, та оновлює регістр і змінні 
  @@:
  cmp edi, [tmpVar.a]
  jb @f
    pushad
    mov eax, [tmpMem.len]
    add eax, 20
    
    mov ebx, eax
    mov ecx, [tmpMem.pMem]
    
    _realloc ecx, eax
    mov [tmpMem.pMem], eax
    
    add ebx, [tmpMem.pMem]
    mov [tmpVar.a], ebx
    
    popad
    mov edi, [tmpMem.pMem]
    jmp @b
  @@:
    
    
  mov word[edi], ax ; = AL | 00
  inc edi
  inc ecx
  mov edx, -1
  
  jmp .getWordLoop
  
  .getWordLoopEnd:
    xor eax, eax
  .exit:
  pop ebx
  add ecx, [_str]
  sub ecx, ebx
  
ret
endp

proc testSpecSimbol
  ; In:
  ; [fileIn.pMem] - input string from 'getWord'
  
  ; Out:
  ; EDX - bool '=' - знак 'дорівнює' вказує, що тепер потрібно записати рядок у [nowcom.valueAdr]

  xor edx, edx
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
      mov edx, true                     
      inc [fileIn.index]
      jmp .cmdSpecSimbol
    @@:
    
    add esp, 4 ; clear stack
    jmp lexer.getWordLoop
  .cmdSpecSimbol:
ret
endp

proc testDatatype
  ; In:
  ; [fileIn.pMem] - input string from 'getWord'
  
  ; Out:
  ; ECX - index datatype

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
ret
endp

proc getTypeCom, _equ

  cmp [nowcom.name], 0
  jne @f
    mov eax, error.waitName
    jmp error.lexer
  @@:
  
  mov al, byte[_equ]
  cmp al, true
  jne @f
    cmp [nowcom.value], 0
    jne @f
      mov eax, error.waitValue
      jmp error.lexer
  @@:

ret
endp

proc createTocken
  ; Функція створює токен, заповнює структуру токена, тд.
  
  ; In:
  ; struct nowcom
  ; struct pTocken
  ; tockenIndex
  
  ; Out:
  ; struct pTocken



ret
endp