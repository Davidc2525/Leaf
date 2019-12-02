funcion suma( a , b: numero ) :numero
  
  retornar a mas b
  
fin


funcion main:numero
  x: numero = 0
  puntero contador: numero = ref de x // o nuevo numero(0)
  
  si suma(2,2) mayor o igual 4 
   escribir("2+2 mayor a 4")
  sino
    escribir("2+2 menor a 4")
  fin
  
  
  mientras ( des contador ) menor 10
      
      escribir("x = "+nac( incrementar (des contador)))//nac = numero a cadena
      
  fin 
  
  retornar 0
  
fin
