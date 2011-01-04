<?xml version="1.0"?>
<xsl:stylesheet version = "1.0"
    xmlns:xsl = "http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text"/>
<xsl:strip-space elements = "CATALOG"/>
<xsl:template match="PLANT">
<xsl:text/><xsl:value-of select="normalize-space(PLANT)"/>"<xsl:text/>
<xsl:value-of select="normalize-space(COMMON)"/>";"<xsl:text/>
<xsl:value-of select="normalize-space(BOTANICAL)"/>";"<xsl:text/>
<xsl:value-of select="normalize-space(ZONE)"/>";"<xsl:text/>
<xsl:value-of select="normalize-space(LIGHT)"/>";"<xsl:text/>
<xsl:value-of select="normalize-space(PRICE)"/>";"<xsl:text/>
<xsl:value-of select="normalize-space(AVAILABILITY)"/>"&#10;<xsl:text/>
<xsl:text disable-output-escaping = "yes" >
</xsl:text>
</xsl:template>
</xsl:stylesheet>