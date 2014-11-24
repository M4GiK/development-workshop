<?
	session_start();
	//session_register("admin");
	
	if(!isset($_SESSION["admin"]))
			header("location:login.php");
	else
			header("location:executions/index.php");	
		
	
	
?>	
<? 
	include "config.php";
	include "../management/incidenceManager.php";
	
	$im = new incidenceManager();
	$num = $im->getNumberWaitingIncidences();
?>		
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Panel Administracio&oacute;n</title>
</head>

<body>
<ul>
<li> <a href="content/index.php">Gestor de Contenidos</a></li>
<li> <a href="users/index.php">Gestor de Usuarios</a></li>
<li> <a href="news/index.php">Gestor de Noticias</a></li>
<li> <a href="executions/index.php">Gestor de Ejecuciones</a></li>
<li> <a href="events/index.php">Gestor de Eventos</a></li>
<li> <a href="incidences/index.php">Incidencias(<? echo $num;?>)</a></li>
<li> <a href="configuration/index.php">Configuracion</a></li>
<li> <a href="../index.php?section=1">Inicio</a></li>
<li> <a href="index.php?logout=true">Logout</a></li>
</ul>

</body>
</html>
