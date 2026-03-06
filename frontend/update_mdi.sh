name=$( cat $(find ./src) 2>/dev/null | grep -oE '\$mdi\w+' | sort | uniq )
old=$( cat ./src/mdi_aliases.ts | grep -oE '\mdi\w+' | grep -v mdi_aliases | sort | uniq )
exp=$( echo $name | grep -oE '\mdi\w+' | sort | uniq )

sha1=$( echo $old | sha256sum )
sha2=$( echo $exp | sha256sum )
if [[ $sha1 == $sha2 ]]; then
    exit 0;
fi

echo "// This file is auto-generated. Do not edit manually." > ./src/mdi_aliases.ts
echo "// Generated on "$( date ) >> ./src/mdi_aliases.ts
echo "" >> ./src/mdi_aliases.ts
for i in $name; do
    val=${i:1}
    echo "import { $val } from '@mdi/js';" >> ./src/mdi_aliases.ts
done
echo "" >> ./src/mdi_aliases.ts
echo "export const mdi_aliases = {" >> ./src/mdi_aliases.ts
for i in $name; do
    val=${i:1}
    echo "    $val," >> ./src/mdi_aliases.ts
done
echo "};" >> ./src/mdi_aliases.ts