namespace mASN1.framework.experimental 
{
	using mASN1;
    
	using System.Collections;
	using System; // za Console.

	public class DERDecoder:Visitor  
	{
		private Stack valueOctets; /* ovo bi trebao nazvati DER_stack */
		private Stack instances;
		private bool MISSING_OPTIONAL_INSTANCE=true;

		private ExperimentalObject currentInstance = null;

		private byte[] DEROctets;

		public DERDecoder() 
		{
			valueOctets = new Stack();
			instances   = new Stack();
		}

		public void Init(SimpleDER simpleDER) 
		{
			PushDER(simpleDER);
		}

		private void PushDER(SimpleDER simpleDER) 
		{
			valueOctets.Push(simpleDER);
		}

		private void PushDER(byte[] bytes) 
		{
			PushDER(new SimpleDER(bytes));
		}

		
		private void PushInstance(ExperimentalObject instance) 
		{
			instances.Push(instance);
		}

		private void Push_MISSING_OPTIONAL_INSTANCE() 
		{
			instances.Push(MISSING_OPTIONAL_INSTANCE);
		}

		
		public void AssignDER(byte[] inputDER) 
		{
			DEROctets = inputDER;
			valueOctets.Push(inputDER);
		}

		private void ProcessTAG(ASN1TAG inputTag) 
		{
			
			if (inputTag.IsExplicit()) 
			{
				// TODO - sfiksaj ovo.
				/* ako je TAG explicit - stripaj omotac oko DER-a... */
                /* ako je IMPLICIT - ignoriraj..., ostalo ne bi trebalo biti vazno... */	
			
				SimpleDER der = valueOctets.Pop() as SimpleDER;
				SimpleDER unwrappedDER = new SimpleDER(der.GetValueOctets());

				PushDER(unwrappedDER);

				// sad nastavi dalje - sve dok ne dodjemo do simple taga...
				ProcessTAG(((EXPLICITTag)inputTag).underTag);
			}

			if (inputTag.IsImplicit()) 
			{
				/// Go to the next step...
				ProcessTAG(((IMPLICITTag)inputTag).underTag);
			}
		}


		private SimpleDER PopDER() 
		{
			SimpleDER der = valueOctets.Pop() as SimpleDER;

			return der;
		}
		
		
		public override void Visit(mASN1CommonDenominator ob) 
		{
			ProcessTAG(ob.TAG);
			
			currentInstance = (ob as ExperimentalType).Instance();

			if (currentInstance == null) 
			{
				//DIZI FRKU!
			}
		

			ob.acceptVisitor(this);	
		}

		


		#region Visitor-methods
		
		public override void Visit_BOOLEAN(mASN1CommonDenominator ob) 
		{
          	SimpleDER d = PopDER();

			byte[] bytes = d.GetValueOctets();

			if (bytes[0]==0xff) 
			{
				(currentInstance as BOOLEAN).boolValue = true;
			} 
			else 
			{
				(currentInstance as BOOLEAN).boolValue = false;
			}

			PushInstance(currentInstance);
		}



		public override void Visit_NULL(mASN1CommonDenominator ob) 
		{
			SimpleDER der = PopDER();
			PushInstance(currentInstance);
		}

	
		public override void Visit_ANY(mASN1CommonDenominator ob)
		{
			//TODO: ovo jos nisam ni na papiru!!! :-(
			// probati na AlgorithmIdentifier - the classic example.
			throw new NotImplementedException();
		}

		public override void Visit_INTEGER(mASN1CommonDenominator ob)
		{
			//TODO: ovo jos nisam ni na papiru!!! :-(
			// probati na AlgorithmIdentifier - the classic example.
			throw new NotImplementedException();
		}

		public object ShowResult() 
		{
			return (instances.Pop() as ExperimentalObject);
		}

		#region Visit_SEQUENCE with its aux methods

		private void SEQUENCE_Process_DERs(SEQUENCE inst, SimpleDER[] ders) 
		{
			/* process DERs according to the SEQUENCE instance */
			int i=0,j=0;
			for ( ; i < ders.Length; i++, j++) 
			{
				SimpleDER der = ders[i];
				ComponentTypeInfo currentTI = inst.TypeInfos[j];

				try 
				{
					while (!der.Match(currentTI.Type.TAG)) 
					{
						if (currentTI.DefaultValue != null) 
						{
							PushInstance(currentTI.DefaultValue);
							/* No need for visiting.. */
						}
						else if (currentTI.isOptional) 
						{
							/* this is a marker for later initialization
							 * of parent SEQUENCE instance. */
							Push_MISSING_OPTIONAL_INSTANCE();
						}
						else 
						{
							throw new SEQUENCEComponentMismatchException("SEQUENCE Components mismatch!");
						}
	
						currentTI = inst.TypeInfos[++j];
					}
				}
				catch (IndexOutOfRangeException)
				{
					throw new 
						SEQUENCEComponentMismatchException("SEQUENCE Components mismatch at DER:!"+i.ToString());
				}
			
				ProcessTheseTwo(der, currentTI.Type);
 			}
		}

		private void ProcessTheseTwo(SimpleDER der, ExperimentalType eType) 
		{
			PushDER(der);
			Visit(eType);
		}


		
		public override void Visit_SEQUENCE(mASN1CommonDenominator ob)
		{
			SEQUENCE inst = currentInstance as SEQUENCE;
			SimpleDER[] ders = (valueOctets.Pop() as SimpleDER).Unwrap();

			/* pospajaj odgovarajuce DERove sa njihovim instancama, i sve
			 * gurni u SEQUENCE instancu. */
			SEQUENCE_Process_DERs(inst, ders);

			PushInstance(inst);			
		}


		#endregion Visit_SEQUENCE

		#endregion
	}

	

}