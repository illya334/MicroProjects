alf = "abcdefghijklmnopqrstuvwxyz"

strIn = list( input("Enter text: ") )   # Вхідний текст
offset = int( input("Enter offset: ") ) # Зсув

for i in range( len(strIn) ):
    if( strIn[i] == ' ' ):
        continue

    b = 1
    for j in range( len(alf) ):
        if( alf[j] == strIn[i].lower() ):
            b = 0
            
    if(b == 1):
        continue

    up = 0
    if( strIn[i].isupper() ):
        up = 1
    
    index = offset
    c = strIn[i].lower()
    for j in range( len(alf) ):
        if(alf[j] == c):
            index += j
            break
    
    while( index >= len(alf) ):
        index -= len(alf)

    if( up == 0 ):
        strIn[i] = alf[index]
    else:
        strIn[i] = alf[index].upper()

print( "Result: ", ''.join(strIn) )
    
