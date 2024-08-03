; By zeq52giw 25.03.2024

; Ci compiler

; Основна ідея - компілювати Ci мову у ассемблер (FASM)

; Це спроба розробити цей компілятор на ассемблері FASM.

; TODO - пам'ятка, що зробити.
; LTDI - lazy to do it (ленивий, щоб зробити це) - зазвичай буду ставити такі мітки на місця де можна оптимізувати програму, або це критичний код який працює тільки на потужних ПК,
;         але через те, що довго над цією проблемою б'юся - без результатно, залишаю останій робочий варіант.

; Посилання на LTDI:
;  lexer.inc: createTocken

; TODO: зробити щось типо розширення, наприклад основна задача - це дозволити без особливих проблем змінювати оптимізатор, щоб кожен міг його переробити як йому завгодно.

; FATAL ERROR: Невідома помилка при використанні виділеної пам'яті у createTocken! 

format PE Console 4.0
entry Start

include 'win32a.inc'

include 'structs.inc'


section '.data' data readable writeable

include 'data.inc'

section '.text' code readable executable


include 'filer.inc'
include 'string.inc'
include 'lexer.inc'
include 'error.inc'

Start:

  stdcall openFile, fileInName, fileIn
  stdcall readFile, fileIn
  stdcall closeFile, fileIn

  nop
  stdcall lexer

  ;invoke printf, text, eax, [fileIn.pMem]
  invoke getch

Exit:
  invoke  ExitProcess, 0

section '.idata' import data readable

  library kernel32, 'KERNEL32.DLL',\
          msvcrt, 'msvcrt.dll'

  include 'api/kernel32.inc'

  import msvcrt,\
          printf, 'printf',\
          scanf, 'scanf',\
          getch, '_getch'