<!DOCTYPE html>
<html>
<head>
	<title>Ábrák a cikkhez</title>
	<meta charset="UTF-8">
</head>
<body>
	<?php
		foreach(scandir('.') as $f){
			if(strlen($f)>4 && substr($f,-4)===".png"){
				print("<a href=\"$f\" target=\"_blank\"><img src=\"$f\" width=\"600\" /></a>\n");
			}
		}
	?>
</body>
</html>
