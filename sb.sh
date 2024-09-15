echo -n anf= > sb.dat
tr -d '[x ]' < $1 | tr '12345678\n' 'abcdefgh '  | sed -r 's/[ ]+/+/g' | sed -r 's/\+$//' >> sb.dat
echo >>sb.dat

cat sb.dat
