alf = "abcdefghijklmnopqrstuvwxyz"

def testAlf(c):
    b = 0
    for i in range( len(alf) ):
        if(alf[i] == c):
            b = 1
    if(b == 0):
        return 0
    else:
        return 1

def getIndex(c):
    for i in range( len(alf) ):
        if(alf[i] == c):
            return i
    return -1

def longKey(key, length):
    if(type(key) == str):
        key = list(key)
        
    longKeyStr = list()
    for i in range( length ):
        longKeyStr.append( key[ i % len(key) ] )
    return longKeyStr

def algZip(strIn, key):
    if(type(strIn) == str):
        strIn = list(strIn)

    key = longKey(key, len(strIn))
    strOut = list()
    
    for i in range( len(strIn) ):
        up = 0
        if( strIn[i].isupper() ):
            strIn[i] = strIn[i].lower()
            up = 1
        
        if( testAlf( strIn[i] ) ):
            c = getIndex(strIn[i]) + getIndex(key[i])
            if(c > len(alf)):
                c = c - len(alf)
            
            strOut.append( c )
        else:
            strOut.append( strIn[i] )

        if( type( strOut[i] ) == int ):
            strOut[i] = alf[ strOut[i] ]

        if( up == 1 ):
            strOut[i] = strOut[i].upper()

    return ''.join( strOut )

def algUnZip(strIn, key):
    if(type(strIn) == str):
        strIn = list(strIn)

    key = longKey(key, len(strIn))
    strOut = list()
    
    for i in range( len(strIn) ):
        up = 0
        if( strIn[i].isupper() ):
            strIn[i] = strIn[i].lower()
            up = 1
        
        if( testAlf( strIn[i] ) ):
            c = getIndex(strIn[i]) - getIndex(key[i])
            if(c < 0):
                c = c + len(alf)
            
            strOut.append( c )
        else:
            strOut.append( strIn[i] )

        if( type( strOut[i] ) == int ):
            strOut[i] = alf[ strOut[i] ]

        if( up == 1 ):
            strOut[i] = strOut[i].upper()

    return ''.join( strOut )

while(1):
    d = int(input("Enter action:\n1) encrypt\n2) decipher\n>: "))

    if( d == 1 or d == 2 ):
        strIn = input("Enter string: ")
        key = input("Enter key: ")

        if( d == 1 ):
            print( algZip(strIn, key) )
            print("\n\n")
            
        elif( d == 2 ):
            print( algUnZip(strIn, key) )
            print("\n\n")
            
    


