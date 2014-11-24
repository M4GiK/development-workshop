<?
	session_start();
	if($_GET["logout"] == "true")
	{
		session_unregister("login");
		header("location:../../index.php");
		exit();
	}
	else
	{
		//session_register("login");
		if(!isset($_SESSION["login"]))
			header("location:login.php");
	}		
?>
<?
	include "../../admin/config.php";
	include "../../management/incidenceManager.php";
	include "../../management/configManager.php";
	require "../../lib/phpmailer/class.phpmailer.php";
	include "../../lib/Mail.php";
	
	if(isset($_POST["Submit"]) )
	{
		$im = new incidenceManager();
		if($_POST["incidencia"]!="")	
		{
		
			if($im->addIncidence($_POST["incidencia"], $_SESSION["login"]["username"]))
			{	
				$cm =  new configManager();
				$avisos = $cm->getAvisos();
				if($avisos["incidencia"] == "S")
				{
					$to = $cm->getMail();
					$message = "El usuario:<strong> ".$_SESSION["login"]["username"]."</strong> ha abierto una nueva incidencia<br><br> Administracion WEBGRID"; 
					$alt_message = "El usuario:".$_SESSION["login"]["username"]."</strong> ha abierto una nueva incidencia\n\n Administracion WEBGRID"; 
					$mail = new Mail("WEB GRID",$to,"Aviso: nueva incidencia",$message,$alt_message, "../../lib/phpmailer/");
					$mail->send();
				}	
				header("location:message_incidence.php");
			}	
			else
				$error = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; Problems sending incidence <p/>";
		}
		else
			$error = "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp; Please fill in the incidence text <p/>";				
	}
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Web GRID</title>
<link href="../style-css/style_admin_usuario.css" rel="stylesheet" type="text/css" />
 <!--[if IE 6]>
        <link rel="stylesheet" type="text/css" href="style-css/style_ie6_admin_usuario.css" media="all" charset="utf-8" />
 <![endif]--> 
 <!--[if IE 7]>
        <link rel="stylesheet" type="text/css" href="style-css/style_ie7_admin_usuario.css" media="all" charset="utf-8" />
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
				<li class="li_primermenu"><a href="../../index.php" class="link_primermenu">Home</a></li>
				<li class="li_primermenu"><a href="incidences.php?logout=true" class="link_primermenu">Logout</a></li>
			</ul>
		</div>	
		<div id="destacado_gris">
			<ul>
				<li class="li_datos"><strong><? echo $_SESSION["login"]["username"];?></strong></li>
				<li class="li_datos"><strong> Mail:</strong> <? echo $_SESSION["login"]["mail"]; ?></li>
				<li class="li_datos"><strong> Institution:</strong><? echo $_SESSION["login"]["institucion"];?></li>
				<li class="li_datos"><strong> Last login:</strong><? echo date("G:i d/m/Y",$_SESSION["login"]["ultimo_login"]);?></li>
			</ul>
		</div>
	
	<div id="pestanas"> 
		<ul class="ul_pestana">
			<li class="pestanas"><a href="executions.php">Executions</a></li>
			<li class="pestana_selec"><strong>Incidences</strong></li>
			<li class="pestanas"><a href="preferences.php">Settings</a></strong></li>
		</ul>
	</div>
	<div id="parte_central">
	<div id="menu_interior">
		<ul class="ul_segundomenu">
			<li class="li_segundomenu"><a href="incidences.php">My incidences</a></li>
			<li class="li_segundomenu"><span class="segundomenu_selec">New incidence</span></li>
		</ul>
	</div>	 
<div id="result" align="center" >
<h1>New incidence</h1>
<div id="error"><? echo $error;?></div>
<form id="form1" name="form1" method="post" action="">
  <table width="60%" cellpadding="0" cellspacing="0" class="tabladatos">
    <tr>
      <th class="th_tabladatos"><strong>Incidence</strong>
      <td class="td_tabladatos_PAR"><textarea name="incidencia" cols="40" rows="5" id="incidencia"></textarea></td>
    </tr>
    <tr>
      <td colspan="2" class="td_tabladatos_PAR">&nbsp;</td>
    </tr>
    <tr>
      <td colspan="2" class="td_tabladatos_PAR"><input type="submit" name="Submit" value="Submit" /></td>
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
