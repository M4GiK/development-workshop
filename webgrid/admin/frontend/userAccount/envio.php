<?
  // primero hay que incluir la clase phpmailer para poder instanciar
  //un objeto de la misma
  require "phpmailer/class.phpmailer.php";

  //instanciamos un objeto de la clase phpmailer al que llamamos 
  //por ejemplo mail
  $mail = new phpmailer();

  //Definimos las propiedades y llamamos a los métodos 
  //correspondientes del objeto mail

  //Con PluginDir le indicamos a la clase phpmailer donde se 
  //encuentra la clase smtp que como he comentado al principio de 
  //este ejemplo va a estar en el subdirectorio includes
  $mail->PluginDir = "phpmailer/";

  //Con la propiedad Mailer le indicamos que vamos a usar un 
  //servidor smtp
  $mail->Mailer = "smtp";

  //Asignamos a Host el nombre de nuestro servidor smtp
  $mail->Host = "mail.cosasnuestras.com";

  //Le indicamos que el servidor smtp requiere autenticación
  $mail->SMTPAuth = true;

  //Le decimos cual es nuestro nombre de usuario y password
  $mail->Username = "davidm@cosasnuestras.com"; 
  $mail->Password = "martosex";

  //Indicamos cual es nuestra dirección de correo y el nombre que 
  //queremos que vea el usuario que lee nuestro correo
  $mail->From = "admin@feriasolutions.com";
  $mail->FromName = "Feria solutions";

  //el valor por defecto 10 de Timeout es un poco escaso dado que voy a usar 
  //una cuenta gratuita, por tanto lo pongo a 30  
  $mail->Timeout=30;

  //Indicamos cual es la dirección de destino del correo
  $mail->AddAddress("l.feria@feriasolutions.com");

  //Asignamos asunto y cuerpo del mensaje
  //El cuerpo del mensaje lo ponemos en formato html, haciendo 
  //que se vea en negrita
  $mail->Subject = "Nueva consulta";
  
  //cuerpo del mensaje
  	$empresa = $_POST["empresa"];
	$persona = $_POST["persona"];
	$email = $_POST["email"];
	$telefono = $_POST["telefono"];
	$consulta= $_POST["consulta"];
	
  $mail->Body = "<b>Nueva Consulta</b><br><br>
  				Empresa: $empresa<br>
				Persona contacto: $persona <br>
				Email: $email <br>
				Teléfono: $telefono<br>							
				Consulta: $consulta<br>";
  //Definimos AltBody por si el destinatario del correo no admite email con formato html 
  $mail->AltBody = "$empresa-$persona-$email-$telefono-$consulta";

  //se envia el mensaje, si no ha habido problemas 
  //la variable $exito tendra el valor true
  $exito = $mail->Send();

  //Si el mensaje no ha podido ser enviado se realizaran 4 intentos mas como mucho 
  //para intentar enviar el mensaje, cada intento se hara 5 segundos despues 
  //del anterior, para ello se usa la funcion sleep	
  $intentos=1; 
  while ((!$exito) && ($intentos < 5)) {
	sleep(5);
     	//echo $mail->ErrorInfo;
     	$exito = $mail->Send();
     	$intentos=$intentos+1;	
	
   }
 
		
   if(!$exito)
   {
	echo "Problemas enviando correo electrónico a ".$valor;
	echo "<br/>".$mail->ErrorInfo;	
   }
   else
   {
	/////////////////////// MAIL CONFIRMACION ///////////////////////////////////
	
		$confirmacion = new phpmailer();
		$confirmacion->PluginDir = "phpmailer/";
		$confirmacion->Mailer = "smtp";
		$confirmacion->Host = "mail.cosasnuestras.com";
		$confirmacion->SMTPAuth = true;
		$confirmacion->Username = "davidm@cosasnuestras.com"; 
  		$confirmacion->Password = "martosex";
		$confirmacion->From = "info@feriasolutions.com";
  		$confirmacion->FromName = "Feria solutions";
		
		$confirmacion->Timeout=30;
		$confirmacion->AddAddress($email);
	 	$confirmacion->Subject = "Consulta recibida";
  
  		$confirmacion->Body = "Su consulta ha sido recibida con éxito, en breve nos pondremos en contacto con usted.<br> 
								Muchas gracias y reciba un cordial saludo<br>
								Feria solutions";
		$confirmacion->AltBody = "Su consulta ha sido recibida con éxito, en breve nos pondremos en contacto con usted.<br> 
								Muchas gracias y reciba un cordial saludo<br>
								Feria solutions";
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
			echo "Problemas enviando correo electrónico a ".$valor;
			echo "<br/>".$mail->ErrorInfo;	
   		}
   		else
   		{
			header("Location: http://www.cosasnuestras.com/feriasolutions/index2.php");
   		} 
   
	
   } 
   
   
   
?>
