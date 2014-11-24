<?
	/*
 	* @class newsManager
 	* @brief Manager de los contenidos
	* @autor David Martos Expósito 
	*
	*/
	
	class eventManager
	{
		var $linkdb;
		var $db;
		var $userdb;
		var $passworddb;
 	
		function eventManager()
		{
			$this->userdb = USERDB;
			$this->passworddb = PASSWORDDB;
			$this->db = DB;
			$this->linkdb = mysql_connect (MYSQLMACHINE, $this->userdb, $this->passworddb) 
				or die ('I cannot connect to the database because: ' . mysql_error());
		
			mysql_select_db ($this->db,$this->linkdb) or die ('No puedo seleccionar la bd'.mysql_error());
		
		
		}
		
		function addEvent($info)
		{
			$titulo_es = $info["titulo_es"];
			$titulo_en = $info["titulo_en"];
			$titulo_ca = $info["titulo_ca"];
			$dia = $info["dia"];
			$mes = $info["mes"];
			$year = $info["year"];
			$dia_fin = $info["dia_fin"];
			$mes_fin = $info["mes_fin"];
			$year_fin = $info["year_fin"];
			$evento_es = $info["evento_es"];
			$evento_en = $info["evento_en"];
			$evento_ca = $info["evento_ca"];
			$lugar = $info["lugar"];
			
			
			$sql = "INSERT INTO calendario (dia,mes,year,dia_fin,mes_fin,year_fin,evento_es,evento_en,evento_ca,lugar,titulo_es,titulo_en,titulo_ca)VALUES('$dia','$mes','$year','$dia_fin','$mes_fin','$year_fin','$evento_es','$evento_en','$evento_ca','$lugar','$titulo_es','$titulo_en','$titulo_ca')";
			
			if($result = mysql_query($sql, $this->linkdb))
				return true;
				
			else
				return false;	
		}
		
		function editEvent($info,$id)
		{
			$titulo_es = $info["titulo_es"];
			$titulo_en = $info["titulo_en"];
			$titulo_ca = $info["titulo_ca"];
			$dia = $info["dia"];
			$mes = $info["mes"];
			$year = $info["year"];
			$dia_fin = $info["dia_fin"];
			$mes_fin = $info["mes_fin"];
			$year_fin = $info["year_fin"];
			$evento_es = $info["evento_es"];
			$evento_en = $info["evento_en"];
			$evento_ca = $info["evento_ca"];
			$lugar = $info["lugar"];
			
			$sql = "UPDATE calendario SET titulo_es='$titulo_es', titulo_ca='$titulo_ca',titulo_en='$titulo_en',dia='$dia',mes='$mes',year='$year',dia_fin='$dia_fin',mes_fin='$mes_fin',year_fin='$year_fin',evento_es='$evento_es',evento_en='$evento_en',evento_ca='$evento_ca',lugar='$lugar' WHERE id_evento=$id";
			

			if($result = mysql_query($sql, $this->linkdb))
				return true;
			
			else
				return false;	
		}
		
		function removeEvent($id)
		{
			$sql = "DELETE FROM calendario WHERE id_evento=$id";
			
			if($result = mysql_query($sql, $this->linkdb))
			{	
				return true;
				mysql_free_result($result);
			}
			else
				return false;	
		}
		
		
		
		function getEvent($id)
		{
			$sql = "SELECT * FROM calendario WHERE id_evento=$id";
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$row = mysql_fetch_array($result);
				mysql_free_result($result);	
			}
			
			return $row;
		}
		
		function getEvents()
		{
				$sql = "SELECT * FROM calendario ORDER BY dia,mes,year";
				if($result = mysql_query($sql, $this->linkdb))
				{
					$i = 0;
					while($row = mysql_fetch_array($result))
					{
						$events[$i] = $row;
						$i++;
					}
				
					mysql_free_result($result);	
				}
			
			return $events;
		}
		function getDayEvents($day, $month, $year)
		{
			$sql = "SELECT * FROM calendario WHERE ((dia=$day OR dia=0)AND(mes=$month OR mes=0)AND(year=$year OR year=0)) OR (dia<$day AND dia_fin>=$day AND mes=mes_fin AND mes=$month AND(year=$year OR year=0)) OR (dia<$day AND mes<mes_fin AND mes=$month AND(year=$year OR year=0)) OR (dia_fin>=$day AND mes<mes_fin AND mes_fin=$month AND(year=$year OR year=0)) OR (mes<$month AND mes_fin>$month AND(year=$year OR year=0)) ORDER BY dia,mes,year ASC";	
			
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$i = 0;
				while($row = mysql_fetch_array($result))
				{
					$events[$i] = $row;
					$i++;
				}
				
				mysql_free_result($result);	
			}
			
			return $events;
		}
		
		function getMonthEvents($month,$year)
		{
			
			$sql = "SELECT * FROM calendario WHERE ( mes=$month OR mes=0 OR mes_fin=$month OR (mes < $month AND mes_fin >= $month) ) AND (year=$year OR year=0) ORDER BY dia,mes,year ASC";	
			
			if($result = mysql_query($sql, $this->linkdb))
			{
				$i = 0;
				while($row = mysql_fetch_array($result))
				{
					$events[$i] = $row;
					$i++;
				}
				
				mysql_free_result($result);	
			}
			
			return $events;
		}
		
		function getCalendarEvents($month, $year)
		{
			
			
			$sql = "SELECT * FROM calendario WHERE (mes=$month OR mes=0 OR mes_fin=$month) AND (year=$year OR year=0) GROUP BY dia,dia_fin ORDER BY dia,dia_fin";	
			
			//echo "<br>$sql<br>";
			for($j = 0; $j< 31;$j++)
			{
				$events= array_fill(1,31,false); 
			}	
 			
			if($result = mysql_query($sql, $this->linkdb))
			{
				
				while($row = mysql_fetch_array($result))
				{
					if($row["dia"] == 0)
						$events = array_fill(1,31,true);
					
					if($row["mes"] == $month && $row["mes"] < $row["mes_fin"])
					{
						//principio evento partido en 2 meses diferentes
						for($i = $row["dia"]; $i <= 31; $i++)
						{
							$events[$i] = true;	
						}
					}
					
					else if($row["mes"] < $month && $row["mes_fin"] == $month )
					{
						//final del evento partido en 2 meses diferentes
						for($i = 1; $i <= $row["dia_fin"]; $i++)
						{
							$events[$i] = true;	
						}
					}	
						
					else
					{	
						//eventos mismo mes
						for($i = $row["dia"]; $i <= $row["dia_fin"]; $i++)
						{
							$events[$i] = true;	
						}
					}
				}
				
				mysql_free_result($result);	
			}
			
			return $events;
		}
		
		
	}

?>
