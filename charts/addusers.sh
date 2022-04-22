#!/bin/bash


for ((i=10;i<=70;i++));
do
	((insval=$i+302)) #must equal the last line of the insertion table 
	sed -i "${insval}i\ \ \ \ INSERT INTO \`AuthenticationSubscription\` (\`ueid\`, \`authenticationMethod\`, \`encPermanentKey\`, \`protectionParameterId\`, \`sequenceNumber\`, \`authenticationManagementField\`, \`algorithmId\`, \`encOpcKey\`, \`encTopcKey\`, \`vectorGenerationInHss\`, \`n5gcAuthMethod\`, \`rgAuthenticationInd\`, \`supi\`) VALUES (\'2089510000000$i\', \'5G_AKA\', \'0C0A34601D4F07677303652C046253$i\', \'0C0A34601D4F07677303652C046253$i\', \'{\\\\\"sqn\\\\\": \\\\\"000000000020\\\\\", \\\\\"sqnScheme\\\\\": \\\\\"NON_TIME_BASED\\\\\", \\\\\"lastIndexes\\\\\": {\\\\\"ausf\\\\\": 0}}\', \'8000\', \'milenage\', \'63bfa50ee6523365ff14c1f45f88737d\', NULL, NULL, NULL, NULL, \'2089500000000$i\');" mysql/values.yaml
done

