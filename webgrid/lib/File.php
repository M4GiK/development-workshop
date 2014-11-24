<?
	class File
	{
		var $filename;
		var $type;								
		
		/* constructora*/
		function File($filename,$type)
		{
			$this->filename = $filename;
			$this->type = $type;
			
			
		}
		
		/* parsea el contenido del fichero*/
		function parse()
		{
			$info = $this->read();
			
			switch($this->type)
			{
				case "trace":
								$parse = $this->parseTrace($info);	
								break;
				case "result":	
								$parse = $this->parseResult($info);	
								break;				
			}
			return $parse;
		}
		/* Lee el fichero pasado en la constructora*/
		function read()
		{
			//echo $this->filename;
			$fd = fopen($this->filename, "r");
			if ($fd) 
			{
				$i = 0;
				while (!feof($fd)) 
				{
					$read[$i] = fgets($fd, 4096);
					$i++;
				}
				fclose ($fd);
				
			}
		
			return $read;
		}
		
		/* devuelve el string HTML del fichero*/		
		function getHTML($num="",$css=false)
		{
			$info = $this->parse();
			
			switch($this->type)
			{
				case "trace":
								if($css != true )
									$html = $this->getHTMLTrace($info,$num);
								else
									$html = $this->getHTMLTraceCSS($info,$num);		
								break;
				case "result":	
								if($css != true)
									$html = $this->getHTMLResult($info);
								else
									$html = $this->getHTMLResultCSS($info);		
								break;				
			}
			
			return $html;
		}
		
		/* exporta como PDF el fichero*/
		function exportAsPDF($message)
		{
			
			require('html2fpdf/html2fpdf.php');
			$pdf=new HTML2FPDF();
			$pdf->AddPage();
			$pdf->SetLeftMargin(2.5);
			$pdf->setFont("Arial");
			
			$html = "<p><strong>$message</strong></p>";
			if($this->type == "trace")
			{
				$result = $this->getHTML(0);
				$html.= $result["html"];
				$total = $result["total"];
				
				for($i = 1; $i< $total; $i++)
				{
					$result = $this->getHTML($i);
					$html .= $result["html"];
				}
			}
			else
				$html .= $this->getHTML();
				
			//echo $html;
			$pdf->WriteHTML($html);
			$pdf->Output();
		}
		
		/* exporta como XLS el fichero*/
		function exportAsXLS($trace=0,$message)
		{
			header("Content-type: application/vnd.ms-excel");
			header("Content-Disposition: attachment; filename=excel.xls");
			
			$html = "<p><strong>$message</strong><p>";
			if($this->type == "trace")
			{
				$result = $this->getHTML($trace);
				$html .= $result["html"];
			}
			else
			{
				$html .= $this->getHTML();
			}
			
			echo $html;	 
		}
		
		////////////// METODOS PRIVADOS //////////////////////////
		// SIN CSS
		
		function getHTMLResult($file)
		{
			$out = $this->getResult($file);
			$html = " <table width=\"500\" border=\"1\" cellspacing=\"0\" bordercolor=\"#000000\">
						<tr>
							<td align=\"center\" bgcolor=\"#99CCFF\" colspan=\"6\"><strong>RESULT</strong></td>
						</tr>
						<tr>
							<td colspan=\"6\">&nbsp;</td>
						</tr>
						<tr>
							<td bgcolor=\"#d2d0b8\" colspan=\"6\"><strong>Configuration</strong></td>
						</tr>
							$out[0]
						<tr>
							<td bgcolor=\"#d2d0b8\" colspan=\"6\"><strong>Stats</strong></td>
						</tr>
							$out[1]
					 </table>";	
			 return $html;
		}
		
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
						
					$result[0].="<td bgcolor=\"#efefef\">$key</td><td>$value</td>";
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
						
					$result[1].="<td bgcolor=\"#efefef\">$key</td><td>$value</td>";
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
		
		// CON CSS
		function getHTMLResultCSS($file)
		{
			$out = $this->getResultCSS($file);
			$html = " <table width=\"80%\" cellpadding=\"0\" cellspacing=\"0\" class=\"tabladatos\">
						<tr>
							<th class=\"th_tabladatos\" colspan=\"6\"><strong>Configuration </strong></td>
						</tr>
							$out[0]
						<tr>
							<th class=\"th_tabladatos\" colspan=\"6\"><strong>Stats</strong></td>
						</tr>
							$out[1]
					 </table>";	
			 return $html;
		}
		
		function getResultCSS($file)
		{
			$i = 0;
			$cols = 0;
			$result = array();
			foreach( $file as $key => $value ) 
			{
				if($key == "Makespan" || $key == "Average makespan")
					$i = 20;
				
				if ($i <= 19)
				{
					if ($cols == 0)
						$result[0].="<tr>";
						
					$result[0].="<td class=\"td_tabladatos_IMPAR\">$key</td><td class=\"td_tabladatos_PAR\">$value</td>";
					$cols++;
					//$i++;
					
					if($cols == 3)
					{	
						$cols = 0;
						$result[0].="</tr>";	
					}
					
					if($i == 20)
					{
						$colspan = 6 - 2*$cols; 
						$result[0].= "<td colspan=\"$colspan\" class=\"td_tabladatos_PAR\">&nbsp;</td>";
						$result[0].="</tr>";
						$cols = 0;
					} 	
				}
				else
				{
					if ($cols == 0)
						$result[1].="<tr>";
						
					$result[1].="<td class=\"td_tabladatos_IMPAR\">$key</td><td class=\"td_tabladatos_PAR\">$value</td>";
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
		
		// SIN CSS
		function getHTMLTrace($parsed,$trace)
		{	
		
			$read = $parsed[$trace];
			$out["total"] = count($parsed);
			$index  = $trace + 1;
			$html ="<p>Trace $index / ".$out["total"] ."</p>";
			$html .= "<table width=\"500\" border=\"1\" cellspacing=\"0\" bordercolor=\"#000000\">
  					<tr>
   					 <td align=\"center\" bgcolor=\"#d2d0b8\"><strong>Time </strong></td>
    				 <td align=\"center\" bgcolor=\"#d2d0b8\"><strong>Event</strong></td>
    				 <td align=\"center\" bgcolor=\"#d2d0b8\"><strong>Information</strong></td>
  				   </tr>";
		
		
			for($i = 0; $i < count($read); $i++)
			{
				$time = $read[$i]["time"];
				$event = $read[$i]["event"];
				$html.="<tr>";
				$html.="<td>$time</td><td>$event</td>";
				$html.= "<td>";
				for($j = 0; $j < count($read[$i]["info"]); $j++)
				{
					$info = $read[$i]["info"][$j];
					$html.="$info<br/>";
				}
				$html.="</td></tr>";
			}
			$html.="</table>";		   
  
  			$out["html"] = $html;
			

			return $out;
		}
		
		//con CSS
		function getHTMLTraceCSS($parsed,$trace)
		{	
		
			$read = $parsed[$trace];
			$out["total"] = count($parsed);
			$index  = $trace + 1;
			$html ="<p><strong>Trace $index / ".$out["total"] ."</strong></p>";
			$html .= "<table width=\"80%\" cellpadding=\"0\" cellspacing=\"0\" class=\"tabladatos\">
  					<tr>
   					 <th class=\"th_tabladatos\"><strong>Time </strong></td>
    				 <th class=\"th_tabladatos\"><strong>Event</strong></td>
    				 <th class=\"th_tabladatos\"><strong>Information</strong></td>
  				   </tr>";
		
		
			for($i = 0; $i < count($read); $i++)
			{
				$time = $read[$i]["time"];
				$event = $read[$i]["event"];
				
				if($i % 2 != 0)
					$class= "td_tabladatos_IMPAR"; 
				else	
					$class= "td_tabladatos_PAR"; 
				
				$html.="<tr>";
				$html.="<td class=\"$class\">$time</td><td class=\"$class\">$event</td>";
				$html.= "<td class=\"$class\">";
				for($j = 0; $j < count($read[$i]["info"]); $j++)
				{
					$info = $read[$i]["info"][$j];
					$html.="$info<br/>";
				}
				$html.="</td></tr>";
			}
			$html.="</table>";		   
  
  			$out["html"] = $html;
			

			return $out;
		}
		
		function parseResult($read)
		{
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
			$traces = array();
			$result = array();
			$i = 2;
			$n = 0;
			while($i < count($read) -2 )
			{
				//$timeline = ereg_replace(" +", " ", $jobline);
				$timeevent = explode("info:",$read[$i]);
				
				if($timeevent[0] == $read[$i])
				{
					//linea de info
					array_push($result[$n-1]["info"],trim($timeevent[0]));
					$i++;
					
					// todas las lineas de info
					//$timeline = ereg_replace(" +", " ", $jobline);
					$timeevent = explode("info:",$read[$i]);
					while($timeevent[0] == $read[$i] && $i < count($read) - 2 )
					{
						
						array_push($result[$n-1]["info"],trim($timeevent[0]));
						
						$i++;
						//$timeline = ereg_replace(" +", " ", $jobline);
						$timeevent = explode("info:",$read[$i]);
					} 
					
				
					
				}
				else
				{					
					//nueva linea de tiempo entera
					list($time, $event, $info) = sscanf($read[$i] ,"time= %s event= %s info: %s");
			
					if(isset($lasttime))
					if(trim($time) == "0.0000" && trim($lasttime) != "0.0000" && $lasttime!="")
					{
					
						$result[$n-1]["info"] = array_slice($result[$n-1]["info"],0,1);
						array_push($traces,$result);
						$n = 0;
						$result = array();
					}
					
					$result[$n]["time"] = trim($time);
					$result[$n]["event"]= trim($event);
					$result[$n]["info"] = array();
					array_push($result[$n]["info"], trim($timeevent[1]));
					$lasttime = $time;
					$n++;
					$i++;
				}
			
			}
			if( $i == count($read) - 2)
			{	
				//echo "se acabaron las trazas";
				array_push($traces,$result);
			}
			//echo "<pre>";
			//print_r($traces);
			//echo "<pre>";
			return $traces;		
		}
	
	}

?>