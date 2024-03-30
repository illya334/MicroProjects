; Цей файл бере WinAPI функції, та дає швидкий доступ до файлів.
; Used WinAPI functions:
; 

proc openFile uses ebx ecx edx esi edi, path, fileSt
  
  mov eax, [path]
  mov esi, [fileSt]
  invoke CreateFileA, eax, GENERIC_READ or GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0
  cmp eax, INVALID_HANDLE_VALUE
  jne @f
    invoke GetLastError
    mov ebx, eax
    mov eax, error.CreateFileA
    jmp error.func
  @@:
  mov [esi], eax
  
  invoke GetFileSize, eax, 0
  mov [esi + 4*4], eax ; [fileIn.fileSize]
  
  mov ebx, eax
  
  inc eax
  invoke LocalAlloc, LPTR, eax
  mov [esi + 4*2], eax ; [fileIn.pMem]
  mov [esi + 4*3], ebx ; [fileIn.lenMem]
  
  invoke LocalHandle, eax
  mov [esi + 4], eax ; [fileIn.hMem]
  
ret
endp

proc readFile uses ebx ecx edx esi edi, fileSt
  
  mov esi, [fileSt]
  
  mov eax, [esi]       ;[fileIn.hFile]
  mov ebx, [esi + 4*2] ;[fileIn.pMem]
  mov ecx, [esi + 4*3] ;[fileIn.lenMem]
                                            
  invoke ReadFile, eax, ebx, ecx, cnt, 0
  test eax, eax
  jne @f
    invoke GetLastError
    mov ebx, eax
    mov eax, error.ReadFile
    jmp error.func
  @@:
  
  mov eax, [cnt]
    
ret
endp