name=$( cat $(find ./src) 2>/dev/null | grep -oE '\$mdi\w+' )

declare -A unique_names

echo "// This file is auto-generated. Do not edit manually." > ./src/mdi_aliases.ts
echo "// Generated on "$( date ) >> ./src/mdi_aliases.ts
echo "" >> ./src/mdi_aliases.ts
for i in $name; do
    val=${i:1}
    if ! grep -q "$val" ./src/mdi_aliases.ts; then
        echo "import { $val } from '@mdi/js';" >> ./src/mdi_aliases.ts
        unique_names["$val"]=1
    fi
done
echo "" >> ./src/mdi_aliases.ts
echo "export const mdi_aliases = {" >> ./src/mdi_aliases.ts
for i in ${!unique_names[@]}; do
    echo "    $i," >> ./src/mdi_aliases.ts
done
echo "};" >> ./src/mdi_aliases.ts