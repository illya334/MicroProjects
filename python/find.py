def efind(text, words):
    i=0
    for i in range(0, len(words)):
        t = text.find( words[i] )
        if( t != -1 ):
            return t
          
    return len(text)

file = open("A.txt", "r")
str = file.read()

substring = input("Enter what find: ")
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
    l.append( str[ i : str.find("]", i)+1 ] )
    
#print(l)

print("len: ", len(l), "\n")

i = 0
for k in l:
    if( i >= len(l) ):
        break
        
    for j in range(10):
        if( i >= len(l) ):
            break

        print(l[i], end = " ")
        i += 1
            
    print("")