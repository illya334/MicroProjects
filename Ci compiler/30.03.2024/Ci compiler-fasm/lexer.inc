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

  cmp dword[nowcom], 0
  jnz @f
    _alloc nowcom.size
    mov [nowcom], eax
    jmp .loopLexer
  @@:
  
  mov ecx, nowcom.size
  @@:
    mov byte[eax], 0
    inc eax
  loop @b


  .loopLexer:
  mov eax, [fileIn.fileSize]
  mov ebx, [fileIn.index]
  cmp eax, ebx
  jnb @f
    mov eax, -1
    jmp .exit
  @@:
  
  
  mov eax, [fileIn.pMem]
  add eax, [fileIn.index]
  push eax
  
  stdcall testSimbol, [eax]
  test eax, eax
  jnz .testSpecSimbolEnd
    ; �������� �� ����������� ������
    pop eax
    mov al, byte[eax]
    
    cmp al, ' '
    jne @f
      inc [fileIn.index]
      jmp .loopLexer
    @@:
  
    .testSpecSimbolError:
    mov eax, error.invalidSim
    jmp error.lexer
  
  .testSpecSimbolEnd:
  
  stdcall testDatatype
  
  
  .exit:
  
ret
endp

proc testSimbol, _simbol
  
  ; �������� ������ �� �������� �� �� �������
  
  ; In:
  ; _simbol (byte - char) - ������ ���� ������������
  
  ; Out:
  ; eax - true/false - �� ������� ��������?
  
  xor eax, eax
  mov bl, byte[_simbol]
  mov ecx, simbols.len
  
  .loopTestSimbol:
    cmp bl, byte[simbols + ecx]
    jne @f
      mov eax, true
      jmp .endLoopTestSimbol
    @@:
  loop .loopTestSimbol
  .endLoopTestSimbol:
  
  ; return eax (bool)
ret
endp

proc getWord, _str
  ; ������ �����
  
  ; In:
  ; _str - ������� ������
  
  ; Out:
  ; EAX - 0 - Ok; (-1) - End File
  ; EAX - ';', ','
  
  ; ECX - offset _str
  ; [tmpMem.pMem] - output string
  
  local .endAdres dd 0
  
  ; �������� ������ ������, ��� �� ����� �� ������ ���'��.
  mov eax, [tmpMem.len]
  add eax, [tmpMem.pMem]
  mov [.endAdres], eax
  
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
  inc ecx
  mov edx, -1
  
  jmp .getWordLoop
  
  .getWordLoopEnd:
    xor eax, eax
  .exit:
  
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
  mov ebx, [nowcom]
  
  cmp byte[ebx + nowcom.type], 0 ; nowcom.type
  jnz .exit

  cmp dword[ebx + nowcom.name], 0 ; nowcom.name
  jne @f
    mov eax, error.waitName
    jmp error.lexer
  @@:
  
  cmp byte[_equ], true
  jne @f
    cmp dword[ebx + nowcom.value], 0 ; nowcom.value
    jne @f
      mov eax, error.waitValue
      jmp error.lexer
  @@:

  
  cmp byte[ebx + nowcom.datatype], 0 ; nowcom.datatype
  jz .exit
    cmp dword[ebx + nowcom.name], 0 ; nowcom.name
    jnz @f
      mov eax, error.waitName
      jmp error.lexer
    @@:
    
    cmp dword[ebx + nowcom.value], 0 ; nowcom.value
    jz @f
      mov byte[ebx + nowcom.type], 2 ; nowcom.type
      jmp .exit
    @@:
    
    mov byte[ebx + nowcom.type], 1 ; nowcom.type
  .exit:

ret
endp

proc createTocken
  ; ������� ������� �����, �������� ��������� ������, ��.
  
  ; In:
  ; struct nowcom
  ; struct pTocken
  ; tockenIndex
  
  ; Out:
  ; struct pTocken
  xor eax, eax
  xor ecx, ecx
  mov ebx, [nowcom]
  mov al, byte[ebx + nowcom.type] ; nowcom.type
  mov cl, byte[ebx + nowcom.datatype] ; nowcom.datatype
  mov esi, [ebx + nowcom.name]
  mov edi, [ebx + nowcom.value]

  invoke printf, tockenText, eax, ecx, esi, edi
  invoke getch

ret
endp