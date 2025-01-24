
def efind(text, words):
    i=0
    for i in range(0, len(words)):
        t = text.find( words[i] )
        if( t != -1 ):
            return t
          
    return len(text)

file = open("A.txt", "r")
str = file.read()

substring = '['
indices = []
index = -1  # начинаем поиск с начала строки
 
while True:
    # находим следующее вхождение подстроки
    index = str.find(substring, index + 1)
 
    # если вхождение не найдено, выходим из цикла
    if index == -1:
        break
 
    # добавляем индекс в список
    indices.append(index)

l = []
x = ["]", ",", ";", " "]

for i in indices:
    l.append( str[ i+1 : str.find("]", i) ] )
    
    
#print( l )

l2 = []

for i in l:
    s = i[ : efind(i, x) ]
    for j in range(0, 3):
        s = i[ : efind(i, x) ]
        if( len(s) < 4 and s != '' ):
            try:
                l2.append(int(s))
            except:
                t = 0

lf = list(set(l2))        

# printing

print("len: ", len(lf), "\\", len(l), "\n")

i = 0
for k in lf:
    if( i >= len(lf) ):
        break
        
    for j in range(10):
        if( i >= len(lf) ):
            break

        print(lf[i], end = " ")
        i += 1
            
    print("")