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
	include "../../management/userManager.php";
	require "../../lib/phpmailer/class.phpmailer.php";
	
	
	function printErrors($errors)
	{

		for($i = 0; $i < count($errors); $i++)
		{
			$error = $errors[$i];
			echo "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;$error</p>";
		}
		
		
	}
	
	function getOptions( $keys, $values, $selected)
	{
	
			
		for($i = 0; $i < count($keys); $i++)
		{
				if($values!= "")
				{
					if ($values[$i] != $selected)
						echo "<option value=\"$values[$i]\">$values[$i]</option>";
					else
						echo "<option value=\"$values[$i]\" selected=\"selected\">$values[$i]</option>";
				}
				else
				{
					if ($i != $selected)
						echo "<option value=\"$i\">$i</option>";
					else
						echo "<option value=\"$i\" selected=\"selected\">$i</option>";	
				}		
				
		}
	}
	
	function checkDataUser($info)
	{
		$errors = array();
		$index = 0;
		
		
		//comprobacion pais no vacio
		if($info["pais"] == "")
		{
			$errors[$index] = "Please provide a country name";
			$index++;
		}
		//comprobacion institucion no vacia
		if($info["institucion"] == "")
		{
			$errors[$index] = "Please provide your institution name";
			$index++;
		}
		
		//comprobacion mail no vacio
		if($info["mail"] == "")
		{
			$errors[$index] = "Please provide your email";
			$index++;
		}
		else
		{
			//comprobacion sintaxis correo
			if(!ereg("[a-zA-Z0-9._]+@[a-zA-Z0-9.-]+.[a-zA-Z]{2,4}",$info["mail"]) ) 
			{		
				$errors[$index] = "The email you provided is not correct." ;
				$index ++;		
			}
		}	
		
		
		
		return $errors;	
	}
	if (isset($_POST["registro"]))
	{	
		$info = $_POST;
		$userm = new userManager();
		$errors = checkDataUser($_POST); 
		
		if(count($errors) == 0 )
		{
			
			$info["username"] = $_SESSION["login"]["username"];
			$info["password"] = $_SESSION["login"]["password"];
			$info["ultimo_login"] = $_SESSION["login"]["ultimo_login"];
			
			if($userm->editUser($info,$info["username"]))
			{	
				$_SESSION["login"] = $info;
				//sendConfirmation($info["username"],$info["password"],$info["mail"]);
				header("location:message_preferences.php");
				//exit();
			}	
			
			else
				$errors[] = "Could not be edited";//$errors[0] = "El usuario ya existe";
		}
		
	}
	else
		$info = $_SESSION["login"];
	
	function getCountries($selected)
	{
		$userm = new userManager();
		$countries = $userm->getCountries();
		
		for($i = 0; $i< count($countries); $i++)
		{
			$country = $countries[$i]["name"];
			if($country == $selected)
				echo "<option value=\"$country\" selected=\"selected\">$country</option>";
			else
				echo "<option value=\"$country\">$country</option>";		
		}
	}
?>
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
				<li class="li_primermenu"><a href="../../index.php?section=1" class="link_primermenu">Home</a></li>
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
			<li class="pestanas"><a href="incidences.php">Incidences</a></li>
			<li class="pestana_selec"><strong>Settings</strong></li>
		</ul>
	</div>
	<div id="parte_central">
<div id="result" align="center">
<h1 >Settings</h1>
<div id="error"><? printErrors($errors);?></div>

<form id="form1" name="form1" method="post" action="preferences.php">
  <table width="70%" cellspacing="0" cellpadding="0" class="tabladatos">
    <tr>
      <th colspan="2" class="td_tabladatos_IMPAR"><strong>User data </strong>
        </td>    </tr>
    <tr>
      <th class="th_tabladatos">mail*</td>
      <td class="td_tabladatos_PAR"><input name="mail" type="text" id="mail" value="<? echo $info["mail"];?>"/>
      <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos">country * </td>
      <td class="td_tabladatos_PAR"><select name="pais" id="pais" ><? getCountries($info["pais"]);?></select>
      <br /></td>
    </tr>
    <tr>
      <td class="th_tabladatos">state/province</td>
      <td class="td_tabladatos_PAR"><input name="provincia" type="text" id="provincia" value="<? echo $info["provincia"];?>"/>
          <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos">city</td>
      <td class="td_tabladatos_PAR"><input name="ciudad" type="text" id="ciudad" value="<? echo $info["ciudad"];?>"/>
          <br />      </td>
    </tr>
    <tr>
      <th class="th_tabladatos">address</td>
      <td class="td_tabladatos_PAR"><input name="direccion" type="text" id="direccion" value="<? echo $info["direccion"];?>"/>
          <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos">phone</td>
      <td class="td_tabladatos_PAR"><input name="telefono" type="text" id="telefono" value="<? echo $info["telefono"];?>"/>
          <br />      </td>
    </tr>
    <tr>
      <th class="th_tabladatos">web</td>
      <td class="td_tabladatos_PAR"><input name="web" type="text" id="web" value="<? echo $info["web"];?>"/>
          <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos">ocupation</td>
      <td class="td_tabladatos_PAR"><select name="ocupacion" id="ocupacion" >
        <? getOptions(array("Engineer","Researcher","Professor","Ms Student","Ph.D. Student","Undergraduate Student","Other"),
		array("Engineer","Researcher","Professor","Ms Student","Ph.D. Student","Undergraduate Student","Other"),$info["ocupacion"]);?>
      </select>
<br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos">interest</td>
      <td class="td_tabladatos_PAR"><textarea name="interes" cols="30" rows="5" id="interes"><? echo $info["interes"]; ?></textarea><br /></td>
    </tr>
    <tr>
      <td colspan="2" class="td_tabladatos_PAR">&nbsp;</td>
    </tr>
    <tr>
      <td colspan="2" class="td_tabladatos_IMPAR"><strong>Institution data </strong></td>
    </tr>
    <tr>
      <th class="th_tabladatos">name* </td>
      <td align="left" class="td_tabladatos_PAR"><input name="institucion" type="text" id="institucion" value="<? echo $info["institucion"];?>" />
          <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos">country</td>
      <td class="td_tabladatos_PAR"><input name="pais_ins" type="text" id="pais_ins" value="<? echo $info["pais_ins"];?>" />
          <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos">state/province</td>
      <td class="td_tabladatos_PAR"><input name="provincia_ins" type="text" id="provincia_ins" value="<? echo $info["provincia_ins"];?>" />
          <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos"> city</td>
      <td class="td_tabladatos_PAR"><input name="ciudad_ins" type="text" id="ciudad_ins" value="<? echo $info["ciudad_ins"];?>"/>
          <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos">address</td>
      <td class="td_tabladatos_PAR"><input name="direccion_ins" type="text" id="direccion_ins" value="<? echo $info["direccion_ins"];?>"/>
          <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos"> phone</td>
      <td class="td_tabladatos_PAR"><input name="telefono_ins" type="text" id="telefono_ins" value="<? echo $info["telefono_ins"];?>"/>
          <br /></td>
    </tr>
    <tr>
      <th class="th_tabladatos">web</td>
      <td class="td_tabladatos_PAR"><input name="web_ins" type="text" id="web_ins" value="<? echo $info["web_ins"];?>"/>
          <br /></td>
    </tr>
    <tr>
      <td colspan="2" class="td_tabladatos_PAR">&nbsp;</td>
    </tr>
    <tr>
      <td colspan="2" align="center" class="td_tabladatos_PAR"><input name="registro" type="submit" id="registro" value="edit" /></td>
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
