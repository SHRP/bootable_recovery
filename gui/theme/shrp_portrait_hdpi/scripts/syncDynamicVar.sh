#!/sbin/sh
cat > /twres/dynamic/themeData.xml <<EOF
<?xml version="1.0"?>
<recovery>
	<variables>
		<!--Colors-->
		<variable name="primaryColor" value="$primaryColor"/>
        <variable name="secondaryColor" value="$secondaryColor"/>
        <variable name="accentColor" value="$accentColor"/>
        <variable name="backgroundColor" value="$backgroundColor"/>
        <variable name="subBackgroundColor" value="$subBackgroundColor"/>
        <variable name="navbarColor" value="$navbarColor"/>
        <variable name="dashboardTextColor" value="$dashboardTextColor"/>
        <!--Toggles-->
        <variable name="batteryBarEnabled" value="$batteryBarEnabled"/>
        <variable name="statusBarEnabled" value="$statusBarEnabled"/>
        <variable name="batteryIconEnabled" value="$batteryIconEnabled"/>
        <variable name="batteryPercentageEnabled" value="$batteryPercentageEnabled"/>
        <variable name="clockEnabled" value="$clockEnabled"/>
        <variable name="centeredClockEnabled" value="$centeredClockEnabled"/>
        <variable name="cpuTempEnabled" value="$cpuTempEnabled"/>
        <variable name="roundedCornerEnabled" value="$roundedCornerEnabled"/>
        <variable name="navbarBackgroundEnabled" value="$navbarBackgroundEnabled"/>
        <variable name="dashboardSubTintEnabled" value="$dashboardSubTintEnabled"/>
        <variable name="dashboardTextColorEnabled" value="$dashboardTextColorEnabled"/>
        <!--ComponentTypes-->
        <variable name="navbarType" value="$navbarType"/>
        <variable name="batteryType" value="$batteryType"/>
        <variable name="dashboardIconType" value="$dashboardIconType"/>
        <variable name="roundedcornerType" value="$roundedcornerType"/>
	</variables>
</recovery>
EOF
