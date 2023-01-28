studentnumber = input()
query = studentnumber.find("?")
if(query == -1):
	checkdigit = (2*int(studentnumber[0]) + 3*int(studentnumber[1]) + 5*int(studentnumber[2]) + 7*int(studentnumber[3])) % 11
	if(checkdigit == 10):
		checkdigit = "X"
	if(studentnumber[5] == str(checkdigit)):
		print("VALID")
	else:
		print("INVALID")
else:
	if(studentnumber[5] == "?"):
		checkdigit = (2*int(studentnumber[0]) + 3*int(studentnumber[1]) + 5*int(studentnumber[2]) + 7*int(studentnumber[3])) % 11
		if(checkdigit == 10):
			checkdigit = "X"
		print(studentnumber[0:5]+str(checkdigit))
	else:
		studentnumber = list(studentnumber)
		studentnumber[query] = "0"					#Setting the "?" to 0 allows me to evaluate all "?" positions in the first 4 digits in one if statement.
		if(studentnumber[5] == "X"):
			studentnumber[5] = "10"
		moddigit = int(studentnumber[5]) - ((2*int(studentnumber[0]) + 3*int(studentnumber[1]) + 5*int(studentnumber[2]) + 7*int(studentnumber[3])) % 11)		#Here since the position of "?" gets the value 0, it doesn't effect the calculation.
		#Furthermore, I know that the value of moddigit is the value of "?"*("?"'s positional multiplier) [I will call this value T] mod 11, so: (T)mod11=(moddigit)mod11. But unless moddigit is equal to 1, I cannot apply modular multiplicative inverse (MMI) to find "?". So I apply MMI on moddigit to find what value I need to multiply moddigit with to get (multiplier*moddigit)mod11=1
		if(moddigit != 0):		#If moddigit==0 , "?" also has to be 0, no other value value will satisfy the equation (value of "?")*(position multiplier of "?")mod11=0. The value of "?" was already set to 0 for the sake of convenience in line 19. No need to change anything.
			multiplier = moddigit**9 % 11		#I use Fermat's little theorem to find the MMI of moddigit. ((multiplier)*(position multiplier of "?")*(value of"?"))mod11=1.
			positionmultiplier=[2,3,5,7]
			studentnumber[query] = str((multiplier*positionmultiplier[query])**9 % 11)		# Since I know (multiplier) and (position multiplier of "?"), I can apply MMI on the product of these 2 to find (value of "?").
		if(studentnumber[5] == "10"):
			studentnumber[5] = "X"
		print(studentnumber[0]+studentnumber[1]+studentnumber[2]+studentnumber[3]+studentnumber[4]+studentnumber[5])