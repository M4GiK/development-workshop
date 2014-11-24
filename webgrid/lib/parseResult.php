
<?php
	
	// obtiene los valores para el resultado
	function getResult($file)
	{
		$i = 0;
		$cols = 0;
		$result = array();
		foreach( $file as $key => $value ) 
		{
   			
			
			if ($i <= 19)
			{
				if ($cols == 0)
					$result[0].="<tr>";
					
				$result[0].="<td bgcolor=\"#FF0000\">$key</td><td>$value</td>";
				$cols++;
				$i++;
				
				if($cols == 3)
				{	
					$cols = 0;
					$result[0].="</tr>";	
				}
				
				if($i == 20)
				{
					$colspan = 6 - 2*$cols; 
					$result[0].= "<td colspan=\"$colspan\">&nbsp;</td>";
					$result[0].="</tr>";
					$cols = 0;
				} 	
			}
			else
			{
				if ($cols == 0)
					$result[1].="<tr>";
					
				$result[1].="<td bgcolor=\"#FF0000\">$key</td><td>$value</td>";
				$cols++;
				
				if($cols == 3)
				{	
					$cols = 0;
					$result[1].="</tr>";
				}		
			}
		}
		return $result;
	}
	// muestra el resultado en una tabla
	function showResult($file)
	{
		$out = getResult($file);
		echo" <table width=\"600\" border=\"1\" cellspacing=\"0\" bordercolor=\"#000000\">
  				<tr>
    				<td align=\"center\" bgcolor=\"#99CCFF\" colspan=\"6\"><strong>RESULTADO</strong></td>
  				</tr>
 			    <tr>
    				<td colspan=\"6\">&nbsp;</td>
  				</tr>
  				<tr>
    				<td bgcolor=\"#FFCC99\" colspan=\"6\"><strong>Configuraci&oacute;n inicial </strong></td>
  				</tr>
  					$out[0]
  				<tr>
    				<td bgcolor=\"#FFCC99\" colspan=\"6\"><strong>Estad&iacute;stica</strong></td>
  				</tr>
  					$out[1]
			 </table>";	
	}

	// lee un fichero y lo introduce en un array
	function read_file($file)
	{
		$fd = @fopen($file, "r");
		if ($fd) 
		{
    		$i = 0;
			while (!feof($fd)) 
			{
    	    	$read[$i] = fgets($fd, 4096);
				$i++;
    	    }
			fclose ($fd);
			return $read;
		}
	}
	// parser de un array del resultado
	function parse($read)
	{
		// leemos el fichero
		//$read = read_file($file);
		
		$n = 0;
		$result = array();
		
		for($i = 0; $i < count($read); $i++)
		{
			// separamos 2 resultados de la misma linea
			$fstep = explode("]",$read[$i]);
			for ($j = 0; $j < count($fstep) - 1; $j++)
			{
				// separamos valor-resultado
				$sstep = explode("[",$fstep[$j]);
				
				//eliminamos espacios y dos puntos del resultado
				$sstep[0] = trim($sstep[0]);
				$sstep[0] = substr($sstep[0],0,strlen($sstep[0]) - 1);
				
				//eliminamos espacios del valor
				$sstep[1] = trim($sstep[1]);
				
				$result[$sstep[0]] = $sstep[1];
				//$result[$n] = $sstep[1];
				$n++; 		 
			}
		}
		return $result;		
	}
	
	function parseTrace($read)
	{
		$result = array();
		
		$i = 2;
		$n = 0;
		while($i < count($read) -2 )
		{
			$timeline = ereg_replace(" +", " ", $jobline);
			$timeevent = explode("info:",$read[$i]);
			
			if($timeevent[0] == $read[$i])
			{
				//linea de info
				array_push($result[$n-1]["info"],trim($timeevent[0]));
				$i++;
				
				// todas las lineas de info
				$timeline = ereg_replace(" +", " ", $jobline);
				$timeevent = explode("info:",$read[$i]);
				while($timeevent[0] == $read[$i] && $i < count($read) - 2 )
				{
					array_push($result[$n-1]["info"],trim($timeevent[0]));
					
					$i++;
					$timeline = ereg_replace(" +", " ", $jobline);
					$timeevent = explode("info:",$read[$i]);
				} 
			
				
			}
			else
			{
				//nueva linea de tiempo entera
				list($time, $event, $info) = sscanf($read[$i] ,"time= %s event= %s info: %s");
				$result[$n]["time"] = trim($time);
				$result[$n]["event"]= trim($event);
				$result[$n]["info"] = array();
				array_push($result[$n]["info"], trim($timeevent[1]));
				$n++;
				$i++;
			}
		
		}
		
		
	
		return $result;		
	}
	
	function showTrace($read)
	{
		$out = "<table width=\"600\" border=\"1\" cellspacing=\"0\" bordercolor=\"#000000\">
  					<tr>
   					 <td align=\"center\" bgcolor=\"#CCFFFF\"><strong>Instante tiempo </strong></td>
    				 <td align=\"center\" bgcolor=\"#CCFFFF\"><strong>Evento</strong></td>
    				 <td align=\"center\" bgcolor=\"#CCFFFF\"><strong>informacion</strong></td>
  				   </tr>";
		
		for($i = 0; $i <count($read); $i++)
		{
			$time = $read[$i]["time"];
			$event = $read[$i]["event"];
			$out.="<tr>";
			$out.="<td>$time</td><td>$event</td>";
			$out.= "<td>";
			for($j = 0; $j < count($read[$i]["info"]); $j++)
			{
				$info = $read[$i]["info"][$j];
				$out.="$info<br/>";
			}
			$out.="</td></tr>";
		}		   
  
  		return $out;
	}
	
	

?>


