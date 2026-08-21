THERAPRISM_ALPHABET =
"ABCDEFGHIJKLMNOPQRSTUVWXY"

def theraprism_encode(text):
 result = ""

 for character in text.upper():
  if character in THERAPRISM_ALPHABET:
   result += character
  elif character == "":
   result += ""
  else:
   result += "?"

 return result

def theraprism_decode(text):
 result = ""

 for character in text.upper():
  if character in THERAPRISM_ALPHABET:
   result += character
  elif character == "":
   result += ''
  else:
   result += "?"
 return result
  
