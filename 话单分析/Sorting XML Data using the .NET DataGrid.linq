<Query Kind="Program">
  <Reference>&lt;ProgramFiles&gt;\Microsoft Visual Studio 9.0\Common7\IDE\PublicAssemblies\Microsoft.VisualStudio.QualityTools.UnitTestFramework.dll</Reference>
  <Reference>&lt;RuntimeDirectory&gt;\System.XML.dll</Reference>
</Query>

void Main()
{
	
}
protected void BindGrid()
{
	string dataUrl = "books.XML";
	string schemaUrl = "books.XSD";
	
	DataSet ds = new DataSet();
	ds.ReadXMLSchema(Server.MapPath(schemaUrl));
	ds.ReadXML(Server.MapPath(dataUrl));
	
	DataView dv = new DataView(ds.Tables[0]);
	DataTable dt = dv.Table;
	
	//Custom names for the columns
	dt.Columns[0].ColumnName = "Item Description";
	dt.Columns[1].ColumnName = "Item Id";
	dt.Columns[2].ColumnName = "Amount";
	
	//Sorting criteria
	dv.Sort = SortField;
	if (!SortAscending)
	{
		// append "DESC" to the sort field name in order to 
		// sort descending
		dv.Sort += " DESC";    
	}
	   
	XMLDataGrid.DataSource = dv;
	XMLDataGrid.DataBind();
}
// Define other methods and classes here
