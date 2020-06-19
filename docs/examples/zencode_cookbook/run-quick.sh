
Z=zenroom
####################



n=0
tmp=`mktemp`

cat <<EOF  > $tmp
{
   "myFirstObject":{
      "myFirstNumber":1,
	  "myFirstString":"Hello World!",
      "myFirstHex": "616e7976616c7565",
      "myFirstBase64": "SGVsbG8gV29ybGQh",
	  "myFirstUrl64": "SGVsbG8gV29ybGQh",
	  "myFirstBinary": "0100100001101001",
	  "myFirstArray":[
         "String1",
		 "String2"
      ]
   },
   "mySecondObject":{
      "mySecondNumber":2,
      "mySecondArray":[
         "anotherString1",
         "anotherString2"
      ]
   },
   "myThirdObject":{
      "myThirdNumber":3,
      "myThirdArray":[
         "oneMoreString1",
         "oneMoreString2",
         "oneMoreString3"
      ]
   },
   "myFourthObject":{
      "myFourthArray":[
         "oneExtraString1",
         "oneExtraString2",
         "oneExtraString3",
		 "oneExtraString4"
      ]
   }
}
EOF


cat <<EOF | tee givenFullList.zen | $Z -z -a $tmp | tee givenFullList.json
# ---- Arrays
Given I have a valid 'string array' named 'myFirstArray'
# BROKEN: Given I have a 'string array' named 'myFirstArray'   
Given I have a valid 'array' named 'myFirstArray'      
Given I have a valid 'string array' named 'mySecondArray' inside 'mySecondObject'
Given I have a 'myThirdArray' inside 'myThirdObject' 
Given I have a 'myFourthArray'  
# ---- Numbers
Given I have a 'number' named 'myFirstNumber'
Given I have a 'myFirstNumber'
Given I have a 'myFirstNumber' inside 'myFirstObject' 
Given I have a valid 'number' named 'myFirstNumber'
# ---- Strings
Given I have a 'string' named 'myFirstString' 
Given I have a 'string' named 'myFirstString' inside 'myFirstObject' 
# BROKEN: Given I have a valid 'string' named 'myFirstString' 
# ---- Different data types
Given I have a 'hex' named 'myFirstHex'
# BROKEN: Given I have a valid 'hex' named 'myFirstHex'
# Rotti anche tutti i "valid" sugli altri tipi di input
Given I have a 'base64' named 'myFirstBase64'
Given I have a 'binary' named 'myFirstBinary'
Given I have a 'url64' named 'myFirstUrl64'
Then print the 'myFirstString' as 'string'
Then print the 'myFirstHex' as 'hex'
Then print the 'myFirstUrl64' as 'hex'
# BROKEN Then print the 'myFirstNumber' as 'number'
EOF

rm -f $tmp