<?

	class Mail
	{
		var $from;
		var $to;
		var $subject;
		var $message;
		var $alt_message; 
		var $plugin_dir;
		
		function Mail($from,$to,$subject,$message,$alt_message, $plugin_dir)
		{
			$this->from = $from;
			$this->to = $to;
			$this->subject = $subject;
			$this->message = $message;
			$this->alt_message = $alt_message;
			$this->plugin_dir = $plugin_dir;
		}
		
		function send()
		{
			$mailer = new phpmailer();
			$mailer->PluginDir = $this->plugin_dir;//"../../lib/phpmailer/";
			$mailer->Mailer = "smtp";
			$mailer->Host = MAILMACHINE;
			$mailer->SMTPAuth = true;
			$mailer->Username = USERMAIL; 
			$mailer->Password = PASSWORDMAIL;
			$mailer->From = MAIL;
			$mailer->FromName = $this->from;
			
			$mailer->Timeout=30;
			$mailer->AddAddress($this->to);
			$mailer->Subject = $this->subject;
	  
			$mailer->Body = $this->message;
			
			$mailer->AltBody = $this->alt_message;
			
			$exito = $mailer->Send();
			$intentos=1; 
			while ((!$exito) && ($intentos < 5)) 
			{
				sleep(5);
				//echo $mail->ErrorInfo;
				$exito = $mailer->Send();
				$intentos=$intentos++;	
			}
			/*if(!$exito)
			{
				echo "Problemas enviando correo electrnico a ".$valor;
				echo "<br/>".$mail->ErrorInfo;	
			}*/
		}
	}
?>