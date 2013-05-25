<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0" >
  <xsl:output method="xml"/>

  <xsl:template match="/periodics/paper">
    <type>
      <xsl:attribute name="value">
        <xsl:value-of select="type" />
      </xsl:attribute>
      <xsl:apply-templates />
    </type>
  </xsl:template>

  <xsl:template match="/periodics/paper/title">
    <title> <xsl:value-of select="." /> </title>
  </xsl:template>

  <xsl:template match="/periodics/paper/type">
  </xsl:template>

  <xsl:template match="/periodics/paper/monthly">
    <monthly> <xsl:value-of select="." /> </monthly>
  </xsl:template>

  <xsl:template match="/periodics/paper/chars">
    <chars>
      <colored> <xsl:value-of select="colored" /> </colored>
      <size> <xsl:value-of select="size" /> </size>
      <glossy> <xsl:value-of select="glossy" /> </glossy>
      <hasIndex> <xsl:value-of select="hasIndex" /> </hasIndex>
    </chars>
  </xsl:template>
</xsl:stylesheet>
