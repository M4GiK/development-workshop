<?

if(isset($_GET["mes"])&& isset($_GET["year"]))	
{  
		
	$month = $_GET["mes"];
	$year = $_GET["year"];
	
	$meses = array(
					"ES" => array("enero","febrero","marzo","abril","mayo","junio","julio","agosto","septiembre","octubre","noviembre","diciembre"),
					"EN" => array("january","february","march","april","may","june","july","august","september","october","november","december"),	
					"CA" => array("gener","febrer","març","abril","maig","juny","juliol","agost","septembre","octubre","novembre","desembre")
				);
	$em = new eventManager();
	$events = $em->getCalendarEvents($month,$year);
	
	$datelast = getdate(mktime(0,0,0,$month+1,0,$year));
	$fin = $datelast["mday"];
	$res = "<table cellpadding=\"0\" cellspacing=\"0\" class=\"tabla_calendario\" summary=\"Calendario de eventos\">
             <tr>
    <th scope=\"col\" class=\"th_calendario\">L</th>
    <th scope=\"col\" class=\"th_calendario\">M</th>
    <th scope=\"col\" class=\"th_calendario\">X</th>
    <th scope=\"col\" class=\"th_calendario\">J</th>
    <th scope=\"col\" class=\"th_calendario\">V</th>
    <th scope=\"col\" class=\"th_calendario\">S</th>
    <th scope=\"col\" class=\"th_calendariofinal\">D</th>
  </tr>";    
                  
    
	 $datefirst = getdate(mktime(0,0,0,$month,1,$year));
   	 $num_dia = $datefirst["wday"];
    
		$i = 1;
	    while($i <= $fin)
   	  	{
     	  $res .= "<tr>";
       
       		if($i == 1)
       		{
				if($num_dia == 0)
        		    $num_dia = 7;
            
	   			for($j = 1; $j<=$num_dia-1; $j++ )
        	  	{
	            	$res.="<td class=\"td_calendario\">&nbsp;</td>";
        	  	}	
            
	       }   
       
	       for($j=$num_dia; $j<=7; $j++)
		   {
    			if($j != 7)
				{	
					if($events[$i])
    			 		$res.="<td class=\"td_calendarioSelec\"><a href=\"events.php?filter=day&amp;day=$i&amp;month=$month&amp;year=$year\" class=\"a_calendario\">$i</a></td>";
					else
							$res.="<td class=\"td_calendario\">$i</td>";  
				}
				else
				{
					if($events[$i])
    			 		$res.="<td class=\"td_calendarioSelec_final\"><a href=\"events.php?filter=day&amp;day=$i&amp;month=$month&amp;year=$year\" class=\"a_calendario\">$i</a></td>";
					else
							$res.="<td class=\"td_calendariofinal\">$i</td>";  
				
				}	
    			
    			$i++;
    			
				if($i>$fin)
    				break;
			}
    		 
         	$num_dia = 1;   
         	$res .= "</tr>";  
      	}
    
		switch($month)
		{
			case 1:
					$mesant = 12;
					$mespost = 2;
					$yearant = $year -1;
					$yearpost = $year;
					break;
			case 12:
					$mesant = 11;
					$mespost = 1;
					$yearpost = $year + 1;
					$yearant = $year;
					
					break;
			default:
					$mesant = $month - 1;
					$mespost = $month + 1;
					$yearant = $year;
					$yearpost = $year;
					break;					
		}
		
		$mesact = $meses[$_GET["lang"]][$month -1];
		$res.= "<tr> 
				<td colspan=\"7\" class=\"td_mes\"><a class=\"a_calendarioMes\" href=\"$PHP_SELF?month=$mesant&amp;year=$yearant\">&lt;&lt;</a> $mesact - $year <a class=\"a_calendarioMes\" href=\"$PHP_SELF?month=$mespost&amp;year=$yearpost\">&gt;&gt;</a>
				</td>	
			</tr>";
	
		$res.= "</table>";
  
		echo $res;
	
}

?>
