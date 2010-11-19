Module Module1

	Sub Main()

		Dim _db As New integrationDataContext
		Dim _orderfile As XDocument = XDocument.Load("c:\xml\orders.xml")

		Dim _orders = _orderfile.<orders>.<order>

		For Each _order In _orders

			Dim _o As New order

			_o.OrderNumber = _order.@number
			_o.pickup = _order.<pickup>.Value
			_o.deliver = _order.<deliver>.Value
			_o.deliverdate = Date.Parse(_order.<deliverdate>.Value)

			Dim _items = _order.<items>.<item>

			For Each _item In _items
				Dim _i As New item

				_i.code = _item.<code>.Value
				_i.description = _item.<description>.Value
				_i.quantity = Integer.Parse(_item.<quantity>.Value)

				_o.items.Add(_i)

			Next

			_db.orders.InsertOnSubmit(_o)

		Next

		_db.SubmitChanges()
		_db = Nothing
		_orderfile = Nothing
		_orders = Nothing

	End Sub

End Module
