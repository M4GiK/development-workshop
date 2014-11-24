<? 
	include "../../admin/config.php";
	include "../../management/userManager.php";
	require "../../lib/phpmailer/class.phpmailer.php";
	
	function sendConfirmation($username, $password, $mail)
	{
		$confirmacion = new phpmailer();
		$confirmacion->PluginDir = "../../lib/phpmailer/";
		$confirmacion->Mailer = "smtp";
		$confirmacion->Host = "mail.cosasnuestras.com";
		$confirmacion->SMTPAuth = true;
		$confirmacion->Username = USERMAIL; 
  		$confirmacion->Password = PASSWORDMAIL;
		$confirmacion->From = "info@cosasnuestras.com";
  		$confirmacion->FromName = "Grid Web";
		
		$confirmacion->Timeout=30;
		$confirmacion->AddAddress($mail);
	 	$confirmacion->Subject = "Registro con exito";
  
  		$confirmacion->Body = "Su registro ha sido realizado con &eacute;xito<br/>
								Recuerde su usuario y password son los siguientes:<br/><br/>
								<strong>username:</strong> $username<br/>
								<strong>password:</strong> $password<br/><br/>
								
								Un cordial saludo,<br/><br/>
								
								Administraci&oacute;n Grid Web";
		
		$confirmacion->AltBody ="Su registro ha sido realizado con éxito\n
								Recuerde su usuario y password son los siguientes:\n
								username: $username\n
								password: $password\n\n
								Un cordial saludo\n\n
								Grid Web";
		
		$exito = $confirmacion->Send();
 		$intentos=1; 
  		while ((!$exito) && ($intentos < 5)) 
		{
			sleep(5);
			//echo $mail->ErrorInfo;
			$exito = $confirmacion->Send();
			$intentos=$intentos+1;	
		}
		if(!$exito)
   		{
			echo "Problemas enviando correo electrnico a ".$valor;
			echo "<br/>".$mail->ErrorInfo;	
   		}
   		
	}
	function printErrors($errors)
	{
		$n = count($errors);
		if($n > 0)
			echo "<ul>";
		
		for($i = 0; $i < count($errors); $i++)
		{
			$error = $errors[$i];
			echo "<li>$error</li>";
		}
		
		if($n > 0)
			echo"</ul>";
	}
	
	function checkDataUser($info)
	{
		$errors = "";
		$index = 0;
		
		
		//comprobacion usuario no vacio
		if($info["username"] == "")
		{
			$errors[$index] = "Debe especificar un nombre de usuario";
			$index++;
		}
		
		//comprobacion mail no vacio
		if($info["mail"] == "")
		{
			$errors[$index] = "Debe especificar un mail";
			$index++;
		}
		else
		{
			//comprobacion sintaxis correo
			if(!ereg("[a-zA-Z0-9._]+@[a-zA-Z0-9.-]+.[a-zA-Z]{2,4}",$info["mail"]) ) 
			{		
				$errors[$index] = "El email no es válido" ;
				$index ++;		
			}
		}	
		
		//comprobacion password no vacio
		if($info["password"] == "")
		{	
			$errors[$index] = "Debe especificar un password";
			$index++;
		}
		else 
		{	
			//comprobacion password y confirmacion son iguales
			if($info["password"] != $info["retype"])
			{
				$errors[$index] = "El password y la confirmación no coinciden"; 
				$index++;
			}	
		}
		
		
		return $errors;	
	}
	if (isset($_POST["registro"]))
	{
		
		$info["username"] = $_POST["username"];
		$info["password"] = $_POST["password"];
		$info["retype"] = $_POST["retype"];
		$info["mail"] = $_POST["mail"];
		$info["institucion"] = $_POST["institucion"];
		
		$userm = new userManager();
		
		$errors = checkDataUser($info); 
		if($errors == "")
		{
			if($userm->addUser($info))
			{	
				sendConfirmation($info["username"],$info["password"],$info["mail"]);
				header("location:register.php?username=".$info["username"]);
				exit();
			}	
			
			else
				$errors[0] = "El usuario ya existe";
		}
		
		
	}
?>
	
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<title>Registro</title>
</head>

<body>
<h1 align="left">REGISTRO</h1>
<form id="form1" name="form1" method="post" action="formRegister.php">
  <table width="403" cellspacing="0">
    <tr>
      <td colspan="2" align="left" valign="top"><? printErrors($errors);?></td>
    </tr>
    <tr>
      <td width="181" align="center" valign="middle" bgcolor="#FFFFCC">username</td>
      <td width="216" align="left" valign="middle"><input name="username" type="text" id="username" value="<? if (isset($_POST["registro"]))
		{
			echo $info["username"];
		} 
	  
	  ?>" />
      <br /></td>
    </tr>
    <tr>
      <td align="center" valign="middle" bgcolor="#FFFF99">password</td>
      <td align="left" valign="middle"><input name="password" type="password" id="password" />
      <br /></td>
    </tr>
    <tr>
      <td align="center" valign="middle" bgcolor="#FFFFCC">retype password </td>
      <td align="left" valign="middle"><input name="retype" type="password" id="retype" />
      <br /></td>
    </tr>
    <tr>
      <td align="center" valign="middle" bgcolor="#FFFF99">institution</td>
      <td align="left" valign="middle"><input name="institucion" type="text" id="institucion" value="<? if (isset($_POST["registro"]))
		{
			echo $info["institucion"];
		}
	  
	  ?>"/>
      <br /></td>
    </tr>
    <tr>
      <td align="center" valign="middle" bgcolor="#FFFFCC">mail</td>
      <td align="left" valign="middle"><input name="mail" type="text" id="mail" value="<? if (isset($_POST["registro"]))
		{
			echo $info["mail"];
		}
	  
	  ?>"/>
      <br /></td>
    </tr>
    <tr>
      <td colspan="2">&nbsp;</td>
    </tr>
    <tr>
      <td colspan="2" align="center"><input name="registro" type="submit" id="registro" value="registrar" /></td>
    </tr>
  </table>
</form>
<a href="../../index.php">inicio</a>
</body>
</html>
