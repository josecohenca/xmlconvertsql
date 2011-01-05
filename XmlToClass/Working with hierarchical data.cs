Working with hierarchical data:
public class Customer 
{
  public const string XmlName = "customer";
  public int? ID;
  public string FirstName, LastName;

  public Customer() { }
  public Customer (XmlReader r) { ReadXml (r); }

  public void ReadXml (XmlReader r)
  {
    if (r.MoveToAttribute ("id")) ID = r.ReadContentAsInt();
    r.ReadStartElement();
    FirstName = r.ReadElementContentAsString ("firstname", "");
    LastName = r.ReadElementContentAsString ("lastname", "");
    r.ReadEndElement();
  }

  public void WriteXml (XmlWriter w)
  {
    if (ID.HasValue) w.WriteAttributeString ("id", "", ID.ToString());
    w.WriteElementString ("firstname", FirstName);
    w.WriteElementString ("lastname", LastName);
  }
}

public class Supplier 
{
  public const string XmlName = "supplier";
  public string Name;

  public Supplier() { }
  public Supplier (XmlReader r) { ReadXml (r); }

  public void ReadXml (XmlReader r)
  {
    r.ReadStartElement();
    Name = r.ReadElementContentAsString ("name", "");
    r.ReadEndElement();
  }

  public void WriteXml (XmlWriter w)
  {
    w.WriteElementString ("name", Name);
  }
}

public class Contacts
{
  public IList<Customer> Customers = new List<Customer>();
  public IList<Supplier> Suppliers = new List<Supplier>();

  public void ReadXml (XmlReader r)
  {
    bool isEmpty = r.IsEmptyElement;           // This ensures we don't get
    r.ReadStartElement();                      // snookered by an empty
    if (isEmpty) return;                       // <contacts/> element!
    while (r.NodeType == XmlNodeType.Element)
    {
      if (r.Name == Customer.XmlName)      Customers.Add (new Customer (r));
      else if (r.Name == Supplier.XmlName) Suppliers.Add (new Supplier (r));
      else
        throw new XmlException ("Unexpected node: " + r.Name);
    }
    r.ReadEndElement();
  }

  public void WriteXml (XmlWriter w)
  {
    foreach (Customer c in Customers)
    {
      w.WriteStartElement (Customer.XmlName);
      c.WriteXml (w);
      w.WriteEndElement();
    }
    foreach (Supplier s in Suppliers)
    {
      w.WriteStartElement (Supplier.XmlName);
      s.WriteXml (w);
      w.WriteEndElement();
    }
  }
}
