<? 
	include "../../admin/config.php";
	include "../../management/userManager.php";
	require "../../lib/phpmailer/class.phpmailer.php";
	include "../../management/configManager.php";
	include "../../lib/Mail.php";
	
	function printErrors($errors)
	{

		for($i = 0; $i < count($errors); $i++)
		{
			$error = $errors[$i];
			echo "<p align=\"left\"><img src=\"../../frontend/img/icons/exclamation.png\"/>&nbsp;$error</p>";
		}
		
		
	}
	
	function checkDataUser($info)
	{
		$errors = array();
		$index = 0;
		
		
		//comprobacion usuario no vacio
		if($info["username"] == "")
		{
			$errors[$index] = "Please provide a username";
			$index++;
		}
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
				$errors[$index] = " The email you provided is not correct." ;
				$index ++;		
			}
		}	
		
		//comprobacion password no vacio
		if($info["password"] == "")
		{	
			$errors[$index] = "Please provide a password";
			$index++;
		}
		else 
		{	
			//comprobacion password y confirmacion son iguales
			if($info["password"] != $info["retype"])
			{
				$errors[$index] = "The password and the confirmation password don’t match"; 
				$index++;
			}	
		}
		
		
		return $errors;	
	}
	if (isset($_POST["registro"]))
	{
	
		$info = $_POST;
		$userm = new userManager();
		$errors = checkDataUser($info); 
		
		if(count($errors) == 0)
		{
			if($userm->addUser($info))
			{	
				$message_user =  "Your registration is successfully completed<br/>
								Remember that your username and password are:<br/><br/>
								<strong>username:</strong>". $info["username"]."<br/>
								<strong>password:</strong>". $info["password"]."<br/><br/>
								
								Greetings from WEBGRID @ UPC,<br/><br/>";
				$message_alt_user = "Your registration is successfully completed\n
								Remember that your username and password are:\n
								username: ". $info["username"]."\n
								password: ". $info["password"]."\n\n
								Greetings from WEBGRID @ UPC\n\n";
												
				$mailuser = new Mail("WEBGRID",$info["mail"],"Bienvenido",$message_user,$message_alt_user,"../../lib/phpmailer/");
				$mailuser->send();	
				
				$cm = new configManager();
				$avisos = $cm->getAvisos();
				if($avisos["usuario"] == "S")
				{
					$to = $cm->getMail();
					$message = "Nuevo usuario:".$info["username"]." <br><br> Administracion WEBGRID@UPC"; 
					$alt_message = "Nuevo usuario:".$info["username"]." \n\n Administracion WEBGRID@UPC"; 
					$mail = new Mail("WEB GRID@UPC",$to,"Aviso: nuevo usuario",$message,$alt_message, "../../lib/phpmailer/");
					$mail->send();
				}	
				header("location:register.php?username=".$info["username"]);
				exit();
			}	
			
			else
				$errors[0] = " The user exists in the system";
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
				<li class="li_primermenu"><a href="../../index.php?section=1" class="link_primermenu">volver</a></li>
			</ul>
		</div>	
	<div id="parte_central">
<div id="result" align="center">
<h1>Registro</h1>
<div id="error"><? printErrors($errors);?></div>

<form id="form1" name="form1" method="post" action="formRegister.php">
  <table width="70%" cellpadding="0" cellspacing="0" class="tabladatos">
    <tr>
      <td bgcolor="#FFFFCC" class="th_tabladatos">username * </td>
      <td class="td_tabladatos_PAR"><input name="username" type="text" id="username" value="<? if (isset($_POST["registro"]))
		{
			echo $info["username"];
		} 
	  
	  ?>" />
      <br /></td>
    </tr>
    <tr>
      <td class="th_tabladatos">password * </td>
      <td class="td_tabladatos_PAR"><input name="password" type="password" id="password" />
      <br /></td>
    </tr>
    <tr>
      <td class="th_tabladatos">retype password * </td>
      <td class="td_tabladatos_PAR"><input name="retype" type="password" id="retype" />
      <br /> </td>
    </tr>
    <tr>
      <td class="th_tabladatos">mail*</td>
      <td class="td_tabladatos_PAR"><input name="mail" type="text" id="mail" value="<? if (isset($_POST["registro"]))
		{
			echo $info["mail"];
		}
	  
	  ?>"/>
      <br /></td>
    </tr>
    <tr>
      <td class="th_tabladatos">country * </td>
      <td class="td_tabladatos_PAR"><select name="pais" id="pais" ><? getCountries($_POST["pais"]);?></select>
      <br /></td>
    </tr>
    <tr>
      <td class="th_tabladatos">state/province</td>
      <td class="td_tabladatos_PAR"><input name="provincia" type="text" id="provincia" value="<? if (isset($_POST["registro"]))
		{
			echo $info["provincia"];
		} 
	  
	  ?>"/>
      <br /></td>
    </tr>
    <tr>
      <td class="th_tabladatos">city</td>
      <td class="td_tabladatos_PAR"><input name="ciudad" type="text" id="ciudad" value="<? if (isset($_POST["registro"]))
		{
			echo $info["ciudad"];
		} 
	  
	  ?>"/>
      <br />      </td>
    </tr>
    <tr>
      <td class="th_tabladatos">address</td>
      <td class="td_tabladatos_PAR"><input name="direccion" type="text" id="direccion" value="<? if (isset($_POST["registro"]))
		{
			echo $info["direccion"];
		}
	  
	  ?>"/>
      <br /></td>
    </tr>
    <tr>
      <td class="th_tabladatos">phone</td>
      <td class="td_tabladatos_PAR"><input name="telefono" type="text" id="telefono" value="<? if (isset($_POST["registro"]))
		{
			echo $info["telefono"];
		} 
	  
	  ?>"/>
      <br />      </td>
    </tr>
    <tr>
      <td class="th_tabladatos">web</td>
      <td class="td_tabladatos_PAR"><input name="web" type="text" id="web" value="<? if (isset($_POST["registro"]))
		{
			echo $info["web"];
		}
	  
	  ?>"/>
      <br /></td>
    </tr>
    <tr>
      <td align="center" valign="middle" bgcolor="#FFFFCC" class="th_tabladatos">ocupation</td>
      <td align="left" valign="middle" class="td_tabladatos_PAR"><select name="ocupacion" id="ocupacion" >
         <? getOptions(array("Engineer","Researcher","Professor","Ms Student","Ph.D. Student","Undergraduate Student","Other"),
		array("Engineer","Researcher","Professor","Ms Student","Ph.D. Student","Undergraduate Student","Other"),$info["ocupacion"]);?>
      </select>
<br /></td>
    </tr>
    <tr>
      <td class="th_tabladatos">interest</td>
      <td class="td_tabladatos_PAR"><textarea name="interes" cols="30" rows="5" id="interes"><? if (isset($_POST["registro"]))
		{
			echo $info["interes"];
		} 
		?></textarea><br /></td>
    </tr>
    <tr>
      <td colspan="2" class="td_tabladatos_PAR">&nbsp;</td>
    </tr>
    <tr>
      <td colspan="2" class="td_tabladatos_IMPAR"><strong>Institution data </strong></td>
    </tr>
    <tr>
      <td class="th_tabladatos">name* </td>
      <td class="td_tabladatos_PAR"><input name="institucion" type="text" id="institucion" value="<? if (isset($_POST["registro"]))
		{
			echo $info["institucion"];
		} 
	  
	  ?>" />
      <br /></td>
    </tr>
    <tr>
      <td class="th_tabladatos">country</td>
      <td class="td_tabladatos_PAR"><input name="pais_ins" type="text" id="pais_ins" value="<? if (isset($_POST["registro"]))
		{
			echo $info["pais_ins"];
		} 
	  
	  ?>" />
      <br /></td>
    </tr>
    <tr>
      <td class="th_tabladatos">state/province</td>
      <td class="td_tabladatos_PAR"><input name="provincia_ins" type="text" id="provincia_ins" value="<? if (isset($_POST["registro"]))
		{
			echo $info["provincia_ins"];
		} 
	  
	  ?>" />
      <br />      </td>
    </tr>
    <tr>
      <td class="th_tabladatos"> city</td>
      <td class="td_tabladatos_PAR"><input name="ciudad_ins" type="text" id="ciudad_ins" value="<? if (isset($_POST["registro"]))
		{
			echo $info["ciudad_ins"];
		}
	  
	  ?>"/>
      <br /></td>
    </tr>
    <tr>
      <td class="th_tabladatos">address</td>
      <td class="td_tabladatos_PAR"><input name="direccion_ins" type="text" id="direccion_ins" value="<? if (isset($_POST["registro"]))
		{
			echo $info["direccion_ins"];
		}
	  
	  ?>"/>
      <br /></td>
    </tr>
    <tr>
      <td class="th_tabladatos"> phone</td>
      <td class="td_tabladatos_PAR"><input name="telefono_ins" type="text" id="telefono_ins" value="<? if (isset($_POST["registro"]))
		{
			echo $info["telefono_ins"];
		}
	  
	  ?>"/>
      <br /></td>
    </tr>
    <tr>
      <td class="th_tabladatos">web</td>
      <td class="td_tabladatos_PAR"><input name="web_ins" type="text" id="web_ins" value="<? if (isset($_POST["registro"]))
		{
			echo $info["web_ins"];
		}
	  
	  ?>"/>
      <br /></td>
    </tr>
    <tr>
      <td colspan="2" class="td_tabladatos_PAR">&nbsp;</td>
    </tr>
    <tr>
      <td colspan="2" class="td_tabladatos_PAR"><input name="registro" type="submit" id="registro" value="registrar" /></td>
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