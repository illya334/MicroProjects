;   / ---------- \
; < -------------- >
;   \ ---------- /

; By zeq52giw 25.03.2024

; Цей файл перетворює код у токени

; Code => Lexer => Parser => Compiler (asm)

proc lexer
  ; Основна частина Lexer-a. Завдяння цієї подпрограми розбити текст на токени, переобразувати його (якщо потрібно).

  ; In:
  ; fileIn struct - структура входного файла на обробку.

  ; Out:
  ;

  ; TODO:
  ; обробка функцій - переробити lexer так щоб він не використовував nowcom, а щоб записував у відведену для цього пам'ять по адресі

  local .cmdEqu db 0

  ; Init
  cmp [tmpMem.pMem], 0
  je @f
    _free [tmpMem.pMem]
  @@:
  _alloc 40
  mov [tmpMem.pMem], eax
  mov [tmpMem.len], 40

  mov [nowLine], 0
  mov [nowCol], 0
  ; end init

  .loopLexer:

  ; перевірка символа, чи належить він до алфавіту
  stdcall getByteFile, fileIn
  test eax, eax
  jz .fileEnd
  
  stdcall testSimbol, eax
  test ebx, ebx
  jnz .testSpecSimbolEnd
    ; Перевірка на спеціальний символ

    cmp al, ' ' ; якщо space то перепригуєм його та повертаємось в початок
    je .loopLexer

    cmp al, 13
    jne @f
      inc [nowLine]
      mov [nowCol], 0
      jmp .loopLexer
    @@:

    cmp al, 10
    je .loopLexer
    
    cmp al, '"'
    je .string1
    
    cmp al, "'"
    je .string2

    ; if( ( '!' >= bl <= '/' ) || ( ':' >= bl <= '@' ) || ( '[' >= bl <= '_' ) || ( '{' >= bl <= '~' ) ) createTocken(al, null);

    ; if( '!' >= bl <= '/' )
    cmp al, '!'
    jnae @f
      cmp al, '/'
      jnbe @f
    
      stdcall createTocken, eax, 0
      jmp .loopLexer
    @@:
    
    ; if( ':' >= bl <= '@' )
    cmp al, ':'
    jnae @f
      cmp al, '@'
      jnbe @f
    
      stdcall createTocken, eax, 0
      jmp .loopLexer
    @@:

    ; if( '[' >= bl <= '_' )
    cmp al, '['
    jnae @f
      cmp al, '_'
      jnbe @f
    
      stdcall createTocken, eax, 0
      jmp .loopLexer
    @@:
    
    ; if( '{' >= bl <= '~' )
    cmp al, '{'
    jnae @f
      cmp al, '~'
      jnbe @f
    
      stdcall createTocken, eax, 0
      jmp .loopLexer
    @@:

    .testSpecSimbolError: ; невідомий символ - помилка
      mov eax, error.invalidSim
      jmp error.lexer

    .string1:
      mov ebx, [tmpMem.pMem]
      xor ecx, ecx
      @@:
      stdcall getByteFile, fileIn
      cmp al, '"'
      je @f
        mov byte[ebx+ecx], al
        inc ecx
        jmp @b
      @@:
      mov byte[ebx+ecx], 0
      
      stdcall createTocken, 2, [tmpMem.pMem] ; type = string = 2
      jmp .loopLexer
      
    .string2:
      mov ebx, [tmpMem.pMem]
      xor ecx, ecx
      @@:
      stdcall getByteFile, fileIn
      cmp al, "'"
      je @f
        mov byte[ebx+ecx], al
        inc ecx
        jmp @b
      @@:
      mov byte[ebx+ecx], 0
      
      stdcall createTocken, 2, [tmpMem.pMem] ; type = string = 2
      jmp .loopLexer
      
  .testSpecSimbolEnd:

  stdcall getWord, eax ; Виділяє слово у [tmpMem.pMem]

  stdcall testDatatype

  test eax, eax
  jnz @f
    xor ecx, ecx
  @@:

  or cx, (1 shl 7)

  stdcall createTocken, ecx, [tmpMem.pMem]

  stdcall getEOF, fileIn ; Is it end file
  test eax, eax
  jz .fileEnd

  ; stdcall createTocken, 0, 0

  jmp .loopLexer

  .fileEnd:

  invoke printf, fileEndText

  .exit:

ret
endp

proc testSimbol, _simbol

  ; перевіряє символ чи належить він до алфавіту

  ; In:
  ; _simbol (byte - char) - символ який перевіряється

  ; Out:
  ; eax - true/false - чи пройшла перевірку?

  ; used eax ebx ecx


  xor ebx, ebx
  mov cl, byte[_simbol]
  
  ; if( '0' >= bl <= '9' ) return 1;
  cmp cl, '0'
  jnae @f
    cmp cl, '9'
    jnbe @f
    
    inc bl ; return 1
    jmp .exit
  @@:

  ; if( 'A' >= bl <= 'Z' ) return 1;
  cmp cl, 'A'
  jnae @f
    cmp cl, 'Z'
    jnbe @f
    
    inc bl ; return 1
    jmp .exit
  @@:

  ; if( 'a' >= bl <= 'z' ) return 1;
  cmp cl, 'a'
  jnae @f
    cmp cl, 'z'
    jnbe @f
    
    inc bl ; return 1
    jmp .exit
  @@:

  cmp cl, '_'
  jne @f
    inc bl ; return 1
  @@:

  .exit:
  ; return ebx (bool)
ret
endp

proc getWord, _firstSimbol
  ; Виділяє слово

  ; In:
  ; _firstSimbol - перший символ слова

  ; Out:
  ; EAX - 0 - Ok; (-1) - End File

  ; ECX - offset _str
  ; [tmpMem.pMem] - output string

  local .endAdres dd 0

  ; обраховує кінцесу адресу, щоб не вийти за розміри пам'яті.
  mov eax, [tmpMem.len]
  add eax, [tmpMem.pMem]
  mov [.endAdres], eax

  mov edi, [tmpMem.pMem]

  mov eax, [_firstSimbol]
  jmp @f
  
  .getWordLoop:  
  stdcall getByteFile, fileIn
  test eax, eax
  jz .endFile
  stdcall testSimbol, eax
  test ebx, ebx
  jz .getWordLoopEnd

  @@:

  ; переповнення пам'яті, перевиділяє пам'ять - збільшує її розмір на 20 байт,
  ; перераховує кінцеву адресу, та оновлює регістр і змінні
  @@:
  cmp edi, [.endAdres]
  jb @f
    pushad
    add [tmpMem.len], 20
    mov eax, [tmpMem.len]

    mov ebx, eax
    mov ecx, [tmpMem.pMem]

    _realloc ecx, eax
    mov [tmpMem.pMem], eax

    add ebx, [tmpMem.pMem]
    mov [.endAdres], ebx

    popad
    mov edi, [tmpMem.pMem]
    jmp @b
  @@:


  mov word[edi], ax ; = AL | 00
  inc edi

  jmp .getWordLoop

  .endFile:
  mov eax, -1
  jmp .exit

  .getWordLoopEnd:
    xor eax, eax
  .exit:

ret
endp

proc testDatatype
  ; In:
  ; [fileIn.pMem] - input string from 'getWord'

  ; Out:
  ; EAX - zero is none (ноль це значить, що слово не належить до типу даних)
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
    jz @f
    pop edi esi ecx
    add edi, eax
    inc edi
    inc ecx
  jmp @b
  @@:
    pop edi esi ecx
ret
endp

proc createTocken, _type, _str
  ; Функція створює токен, заповнює структуру токена, тд.

  ; LTDI: Токен створюється завжди на 5 байтів, навідь коли це тип данних, або спец. символ
  ;       5 * 1000 = 5000 bytes, якщо у файлі 1000 токенів, це малий код. 
  ;       Це не критично на нових ПК, але дуже критично на старих.

  ; if it is first call
  mov ebx, [pTocken]
  test ebx, ebx
  jnz @f
    _alloc 30
    mov [pTocken], eax
    mov [tockenIndex], 0
    mov ebx, eax
  
  jmp .saveData
  @@:

  ; if it is NOT first call

  invoke LocalHandle, [pTocken]
  invoke LocalLock, eax

  ; EAX = (tockenIndex + 1) * 5
  mov eax, [tockenIndex]
  inc eax
  lea eax, [eax*5]
  
  _realloc dword[pTocken], eax
  mov [pTocken], eax
  mov ebx, eax
  ;mov ebx, [pTocken]
  
  .saveData:
  ; EBX = pTocken + tockenIndex * 5
  mov eax, [tockenIndex]
  lea eax, [eax*5]
  add ebx, eax
  
  mov al, byte[_type]
  mov byte[ebx], al   ; TYPE
  
  mov dword[ebx+1], 0
  
  ; зберігає тільки рядок
  
  ; if type is string
  cmp al, 2
  jne @f
  
    push ebx
    stdcall strcopy, dword[_str], 0, 0
    pop ebx
    mov dword[ebx+1], eax  ; STRING

  @@:

  inc [tockenIndex]

  .exit:

  xor eax, eax
  mov al, byte[ebx]
  invoke printf, tockenText, [tockenIndex], eax, eax, dword[ebx+1]
  
  invoke LocalHandle, [pTocken]
  invoke LocalLock, eax
  
  call error.getLastError
  
  ;invoke getch

ret
endp