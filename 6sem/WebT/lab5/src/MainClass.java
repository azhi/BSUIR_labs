import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.w3c.dom.Document;
import org.xml.sax.SAXException;


public class MainClass {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		if ("--validate".equals(args[0])) {
			try {
				JAXBContext jc = JAXBContext.newInstance("generated");
				Unmarshaller u = jc.createUnmarshaller();
				SchemaFactory sf = SchemaFactory.newInstance(
				javax.xml.XMLConstants.W3C_XML_SCHEMA_NS_URI);
				Schema schema = null;
				try {
					schema = sf.newSchema(new File(args[1]));
				} catch (SAXException e) {
					e.printStackTrace();
				} 
				u.setSchema(schema);
				Object ob = u.unmarshal(new FileInputStream(args[2]));
				
				Marshaller m = jc.createMarshaller();
				m.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, Boolean.TRUE);
				m.marshal(ob, System.out);
			} catch(IOException ex) {
				System.err.println("Sorry, cannot open xml file.");
			} catch(JAXBException ex) {
				System.err.println("Sorry, an error occured while porcesing xml file.");
				ex.printStackTrace();
			}	
		} else if ("--transform".equals(args[0])) {
			Document document;
		    DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		    try {
		      File stylesheet = new File(args[1]);
		      File datafile = new File(args[2]);

		      DocumentBuilder builder = factory.newDocumentBuilder();
		      document = builder.parse(datafile);

		      TransformerFactory tFactory = TransformerFactory.newInstance();
		      StreamSource stylesource = new StreamSource(stylesheet);
		      Transformer transformer = tFactory.newTransformer(stylesource);

		      DOMSource source = new DOMSource(document);
		      StreamResult result = new StreamResult(System.out);
		      transformer.transform(source, result);
		    } catch (TransformerConfigurationException tce) {
		      System.err.print("Transformer Factory error: ");
		      System.err.println(tce.getMessage());
		    } catch (TransformerException te) {
		      System.out.print("Transformation error: ");
		      System.out.println(te.getMessage());
		    } catch (SAXException sxe) {
		      System.err.print("SAX error: ");
		      System.err.println(sxe.getMessage());
		    } catch (Exception e) {
		      e.printStackTrace();
		    }	
		}
	}

}
