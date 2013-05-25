<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0" >
  <xsl:output method="xml"/>

  <xsl:template match="/firearms/gun">
    <origin>
      <xsl:attribute name="value">
        <xsl:value-of select="origin" />
      </xsl:attribute>
      <xsl:apply-templates />
    </origin>
  </xsl:template>

  <xsl:template match="/firearms/gun/model">
    <model> <xsl:value-of select="." /> </model>
  </xsl:template>

  <xsl:template match="/firearms/gun/handy">
    <handy> <xsl:value-of select="." /> </handy>
  </xsl:template>

  <xsl:template match="/firearms/gun/origin">
  </xsl:template>

  <xsl:template match="/firearms/gun/material">
    <material> <xsl:value-of select="." /> </material>
  </xsl:template>

  <xsl:template match="/firearms/gun/TTP">
    <TTP>
      <range> <xsl:value-of select="range" /> </range>
      <aimRange> <xsl:value-of select="aimRange" /> </aimRange>
      <hasCage> <xsl:value-of select="hasCage" /> </hasCage>
      <hasOptics> <xsl:value-of select="hasOptics" /> </hasOptics>
    </TTP>
  </xsl:template>
</xsl:stylesheet>
