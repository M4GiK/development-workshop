<?
	session_start();
	if($_GET["logout"] == "true")
	{
		session_unregister("admin");
		header("location:../../index.php");
		exit();
	}
	else
	{
		//session_register("admin");
		if(!isset($_SESSION["admin"]))
			header("location:../login.php");
	}	
	
	
?>	


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Web GRID</title>
<link href="../../frontend/style-css/style_admin_usuario.css" rel="stylesheet" type="text/css" />
 <!--[if IE 6]>
        <link rel="stylesheet" type="text/css" href="../../frontend/style-css/style_ie6_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]--> 
 <!--[if IE 7]>
        <link rel="stylesheet" type="text/css" href="../../frontend/style-css/style_ie7_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]-->
 
<meta http-equiv="content-language" content="cas" />
</head>

<body>
<div id="contenido">
	<div id="cabecera">
		<div id="logo"><img src="../../frontend/img/cuenta_usuario.gif" alt="Cuenta Usuario" /></div>
	</div>
	<div id="central">
		<div id="primer_menu">
			<ul class="ul_primermenu">
				<li class="li_primermenu"><a href="../executions/index.php" class="link_primermenu">Gesti&oacute;n</a></li>
				<li class="li_primermenu"><a href="../../index.php" class="link_primermenu">Inicio</a></li>
				<li class="li_primermenu"><a href="index.php?logout=true" class="link_primermenu">Logout</a></li>
			</ul>
		</div>	
		
	<div id="pestanas"> 
		<ul class="ul_pestana">
			<li class="pestana_selec"><strong>Max ejecuciones</strong></li>
			<li class="pestanas"><a href="#">Password</a></li>
			<li class="pestanas"><a href="#">Mail</a></li>
			<li class="pestanas"><a href="#">Avisos</a></li>
			<li class="pestanas"><a href="#">Via ejecucion</a></li>

		</ul>
	</div>
	<div id="parte_central">
	
	</div>	 
<div id="result" align="center"></div>
<br />
<br />

	</div>
</div>
<div id="peu">&nbsp;</div>
</div>

</body>
</html>