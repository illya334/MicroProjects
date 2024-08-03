; By zeg52giw 25.03.2024

; Цей файл бере WinAPI функції, та дає швидкий доступ до файлів.
; Used WinAPI functions:
; GetLastError, CreateFileA, GetFileSize, LocalAlloc, LocalHandle, readFile, CloseHandle, SetFilePointer

; Ідея:
; Відкриває файл, читає кластерами та записує у виділену пам'ять, якщо доходить до кінця кластера то завантажує інший.
; При повернені вказівника назад, та вказівник буде на початку кластера, він повинен завантажити попередній кластер.
; Програма зчитає по 1 байту.

; Додати обробку стовбців та рядків (col, rol)

; TODO: Перевірити роботоспособність функцій


; struct stFile{
;   dword hFile,        ; 0 = 0   (offset)
;   dword pMem,         ; 1   4
;   dword fileSize,     ; 2   8
;   dword indexKlaster, ; 3   12
;   dword indexFile     ; 4   16
; }

klasterSize = 20 ; розмір кластера

proc openFile, path, fileSt

  mov eax, [path]
  mov esi, [fileSt]
  invoke CreateFileA, eax, GENERIC_READ or GENERIC_WRITE, FILE_SHARE_READ or FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0
  cmp eax, INVALID_HANDLE_VALUE
  jne @f
    invoke GetLastError
    mov ebx, eax
    mov eax, error.CreateFileA
    jmp error.winapi
  @@:
  mov [esi], eax

  invoke GetFileSize, eax, 0
  mov [esi + 4*2], eax ; [stFile.fileSize]

  invoke LocalAlloc, LPTR, klasterSize
  mov [esi + 4*1], eax ; [fileIn.pMem]
  
  mov dword[esi + 4*3], 0 ; [stFile.indexKlaster]
  mov dword[esi + 4*4], 0 ; [stFile.indexFile]

ret
endp

proc readFile, fileSt

  mov esi, [fileSt]

  mov eax, [esi]       ;[fileIn.hFile]
  mov ebx, [esi + 4*1] ;[fileIn.pMem]

  invoke ReadFile, eax, ebx, klasterSize, cnt, 0
  test eax, eax
  jne @f
    invoke GetLastError
    mov ebx, eax
    mov eax, error.ReadFile
    jmp error.winapi
  @@:

  mov eax, [cnt]

ret
endp

proc closeFile, fileSt

  mov esi, [fileSt]
  mov eax, [esi]
  invoke CloseHandle, eax

ret
endp

proc getByteFile uses ebx esi, _stFile

  ; читає 1 байт з файлу, та переміщує вказівник в перед на 1

  mov esi, [_stFile]
  
  mov eax, dword[esi + 4*4] ; indexFile
  cmp eax, dword[esi + 4*2] ; indexFile > fileSize
  jna @f
    xor eax, eax ; дійшли до кінця - return 0
    jmp .exit
  @@:
  
  mov eax, [esi + 4*1] ; pMem
  mov ebx, [esi + 4*3] ; indexKlaster
  movzx eax, byte[eax + ebx]
  push eax
  
  inc dword[esi + 4*3] ; indexKlaster
  inc dword[esi + 4*4] ; indexFile

  cmp dword[esi + 4*3], klasterSize
  jng @f
    stdcall readFile, esi
    mov dword[esi + 4*3], 0
  @@:

  pop eax
  .exit:
  ; return al
ret
endp

proc retByteFile, _stFile
  
  ; зсуває вказівник назад на 1
  
  mov esi, [_stFile]
  
  mov eax, [esi + 4*3] ; indexKlaster
  test eax, eax
  jnz @f
    mov [esi + 4*3], klasterSize ; indexKlaster
    dec [esi + 4*4] ; indexFile
    
    invoke SetFilePointer, esi, -klasterSize, 0, FILE_CURRENT
    
    jmp .exit
  @@:
  
  dec [esi + 4*3] ; indexKlaster
  dec [esi + 4*4] ; indexFile
  
  .exit:
  
ret
endp

proc getEOF, _stFile
  
  ; return:
  ; EAX = 1 - can read, is not end file
  ; EAX = 0 - can't read, is end file
  
  mov esi, [_stFile]
  
  mov eax, true
  mov eax, dword[esi + 4*4] ; indexFile
  cmp eax, dword[esi + 4*2] ; indexFile > fileSize
  jna @f
    xor eax, eax ; дійшли до кінця - return 0
  @@:
ret
endp