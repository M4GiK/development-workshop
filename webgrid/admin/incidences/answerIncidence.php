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
        <link rel="stylesheet" type="text/css" href="style-css/style_ie6_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]--> 
 <!--[if IE 7]>
        <link rel="stylesheet" type="text/css" href="style-css/style_ie7_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]-->
 
<meta http-equiv="content-language" content="cas" />
</head>

<body>
<?
	include "../config.php";
	include "../../management/incidenceManager.php";
	include "../menus.php";
	
	$inc = $_GET["incidence"];
	$im = new incidenceManager();
	$info = $im->getIncidence($inc);
	

?>
<div id="contenido">
	<div id="cabecera">
		<div id="logo"><? include "../header.htm"?></div>
	</div>
	<div id="central">
		<div id="primer_menu">
			<ul class="ul_primermenu">
				<? getFirstMenuGestion();?>
			</ul>
		</div>	
		
	<div id="pestanas"> 
		<ul class="ul_pestana">
			<? getTabsGestion("Incidencias",$num);?>
		</ul>
	</div>
	<div id="parte_central">
	<div id="menu_interior">
		<ul class="ul_segundomenu">
			<li class="li_segundomenu"><a href="index.php">Volver</a></li>
		</ul>
	</div>	 
<div id="result" align="center">
<h1 align="center">Editar incidencia<br/></h1>

<form action="saveIncidence.php" method="post" name="form1" id="form1">
  <table width="80%" cellpadding="0" cellspacing="0" class="tabladatos">
    <tr>
      <th class="th_tabladatos" scope="row"><strong>id</strong></td>
      <td class="td_tabladatos_PAR"><? echo $info["id_incidencia"];?></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>fecha</strong></td>
      <td class="td_tabladatos_PAR"><? echo date("G:i d/m/Y",$info["fecha"]);?></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>usuario</strong></td>
      <td class="td_tabladatos_PAR"><? echo $info["usuario"];?></td>
    </tr>
	 <tr>
      <th class="th_tabladatos" scope="row"><strong>incidencia</strong></td>
      <td class="td_tabladatos_PAR"><? echo $info["incidencia"];?></td>
	 </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>status</strong></td>
      <td class="td_tabladatos_PAR"><? $status.="<select name=\"status\">";
		
      	if ($info["status"] == "espera")
	  		$status.="<option value=\"espera\" selected=\"selected\">espera</option>";
			
		else
			$status.="<option value=\"espera\">espera</option>";
		
		if ($info["status"] == "ok")	
			$status.="<option value=\"ok\" selected=\"selected\">ok</option>";
		else
			$status.="<option value=\"ok\">ok</option>";
		
		if ($info["status"] == "procesando")
	  		$status.="<option value=\"procesando\" selected=\"selected\">procesando</option>";	
		else
			$status.="<option value=\"procesando\">procesando</option>";
		$status.="</select>";
		
		echo $status;
	?>
  <br />
  <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos" scope="row"><strong>respuesta</strong></td>
      <td class="td_tabladatos_PAR"><textarea name="respuesta" cols="80" rows="10" id="respuesta"><? echo $info["respuesta"];?></textarea>
      <br /></td>
    </tr>
    <tr>
      <td class="td_tabladatos_PAR">&nbsp;</td>
      <td class="td_tabladatos_PAR">
          <input type="hidden" name="incidence" value="<? echo $inc;?>"/>
     </td>
    </tr>
    <tr>
      <td class="td_tabladatos_PAR">&nbsp;</td>
      <td class="td_tabladatos_PAR"><input type="submit" name="Submit" value="guardar" /></td>
    </tr>
  </table>
</form>
</div>
<br />
<br />

	</div>
</div>
<div id="peu">&nbsp;</div>
</div>
</body>
</html>

