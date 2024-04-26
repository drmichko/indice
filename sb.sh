echo -n anf= > sb.dat
tr '12345678\n' 'abcdefgh '  < sb.txt | sed -r 's/[ ]+/+/g' | sed -r 's/\+$//' >> sb.dat
echo >>sb.dat

cat sb.dat
