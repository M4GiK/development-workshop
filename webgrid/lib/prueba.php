<?
	require("html2fpdf/html2fpdf.php");
	$html="<table width=\"400\" border=\"1\" >
		<tr>
			<td bgcolor=\"#FF0000\"><strong>nombre</strong></td>
			<td align=\"center\">valor</td>
		</tr>
		<tr>
			<td align=\"center\" bgcolor=\"#FF0000\">nombre2</td>
			<td>valor2</td>
		</tr>
		
		<tr>
			<td colspan=\"2\">Hola </td>	
		</tr>
		</table>";

$pdf = new HTML2FPDF();
$pdf->AddPage();
$pdf->WriteHTML($html);
$pdf->Output();

?>
