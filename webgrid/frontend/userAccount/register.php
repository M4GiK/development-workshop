<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Web GRID</title>
<link href="../style-css/style_admin_usuario.css" rel="stylesheet" type="text/css" />
 <!--[if IE 6]>
        <link rel="stylesheet" type="text/css" href="../style-css/style_ie6_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]--> 
 <!--[if IE 7]>
        <link rel="stylesheet" type="text/css" href="../style-css/style_ie7_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]-->
 
<meta http-equiv="content-language" content="cas" />
</head>
<body>
<div id="contenido">
	<div id="cabecera">
		<div id="logo"><img src="../img/cuenta_usuario.gif" alt="Cuenta Usuario" /></div>
	</div>
	<div id="central">
		<div id="primer_menu">
			<ul class="ul_primermenu">
				<li class="li_primermenu"><a href="../../index.php?section=1" class="link_primermenu">volver</a></li>
			</ul>
		</div>	
	<div id="parte_central">
<div id="result" align="center">
<h1>Registro</h1>
<?
	 



 


	
	$user = $_GET["username"];	
	echo "<p align=\"center\"><img src=\"../../frontend/img/icons/accept.png\"/>&nbsp;Congratulations $user, your registration is successfully completed. </p>";
	
	echo"<p align=\"center\"><img src=\"../../frontend/img/icono_amarill_square.gif\"/><a href=\"../../index.php\">&nbsp;Home page</a></p>";
	
	echo"<p align=\"center\"><img src=\"../../frontend/img/icono_amarill_square.gif\"/><a href=\"login.php\">&nbsp;go to my account</a></p>";
	

		
	
?>
</div>
<br />
<br />

	</div>
</div>
<div id="peu">&nbsp;</div>
</div>
</body>
</html>