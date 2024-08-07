;   / ---------- \
; < -------------- >
;   \ ---------- /

; By zeq52giw 25.03.2024

; ��� ���� ���������� ��� � ������

; Code => Lexer => Parser => Compiler (asm)

proc lexer
  ; ������� ������� Lexer-a. �������� ���� ����������� ������� ����� �� ������, �������������� ���� (���� �������).

  ; In:
  ; fileIn struct - ��������� �������� ����� �� �������.

  ; Out:
  ;

  ; TODO:
  ; ������� ������� - ���������� lexer ��� ��� �� �� �������������� nowcom, � ��� ��������� � �������� ��� ����� ���'��� �� �����

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

  ; �������� �������, �� �������� �� �� �������
  stdcall getByteFile, fileIn
  test eax, eax
  jz .fileEnd
  
  stdcall testSimbol, eax
  test ebx, ebx
  jnz .testSpecSimbolEnd
    ; �������� �� ����������� ������

    cmp al, ' ' ; ���� space �� ���������� ���� �� ����������� � �������
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

    .testSpecSimbolError: ; �������� ������ - �������
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

  stdcall getWord, eax ; ������ ����� � [tmpMem.pMem]

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

  ; �������� ������ �� �������� �� �� �������

  ; In:
  ; _simbol (byte - char) - ������ ���� ������������

  ; Out:
  ; eax - true/false - �� ������� ��������?

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
  ; ������ �����

  ; In:
  ; _firstSimbol - ������ ������ �����

  ; Out:
  ; EAX - 0 - Ok; (-1) - End File

  ; ECX - offset _str
  ; [tmpMem.pMem] - output string

  local .endAdres dd 0

  ; �������� ������ ������, ��� �� ����� �� ������ ���'��.
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

  ; ������������ ���'��, ���������� ���'��� - ������ �� ����� �� 20 ����,
  ; ���������� ������ ������, �� ������� ������ � �����
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
  ; EAX - zero is none (���� �� �������, �� ����� �� �������� �� ���� �����)
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
  ; ������� ������� �����, �������� ��������� ������, ��.

  ; LTDI: ����� ����������� ������ �� 5 �����, ����� ���� �� ��� ������, ��� ����. ������
  ;       5 * 1000 = 5000 bytes, ���� � ���� 1000 �������, �� ����� ���. 
  ;       �� �� �������� �� ����� ��, ��� ���� �������� �� ������.

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
  
  ; ������ ����� �����
  
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