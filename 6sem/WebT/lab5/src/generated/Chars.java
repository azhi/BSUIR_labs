//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, v2.2.4-2 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2013.05.13 at 06:31:38 PM FET 
//


package generated;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for anonymous complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType>
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element ref="{}colored"/>
 *         &lt;element ref="{}size"/>
 *         &lt;element ref="{}glossy"/>
 *         &lt;element ref="{}hasIndex"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "", propOrder = {
    "colored",
    "size",
    "glossy",
    "hasIndex"
})
@XmlRootElement(name = "chars")
public class Chars {

    protected boolean colored;
    protected int size;
    protected boolean glossy;
    protected boolean hasIndex;

    /**
     * Gets the value of the colored property.
     * 
     */
    public boolean isColored() {
        return colored;
    }

    /**
     * Sets the value of the colored property.
     * 
     */
    public void setColored(boolean value) {
        this.colored = value;
    }

    /**
     * Gets the value of the size property.
     * 
     */
    public int getSize() {
        return size;
    }

    /**
     * Sets the value of the size property.
     * 
     */
    public void setSize(int value) {
        this.size = value;
    }

    /**
     * Gets the value of the glossy property.
     * 
     */
    public boolean isGlossy() {
        return glossy;
    }

    /**
     * Sets the value of the glossy property.
     * 
     */
    public void setGlossy(boolean value) {
        this.glossy = value;
    }

    /**
     * Gets the value of the hasIndex property.
     * 
     */
    public boolean isHasIndex() {
        return hasIndex;
    }

    /**
     * Sets the value of the hasIndex property.
     * 
     */
    public void setHasIndex(boolean value) {
        this.hasIndex = value;
    }

}